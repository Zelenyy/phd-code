import abc
from dataclasses import dataclass
from typing import List
from tqdm import tqdm
import numpy as np
from phd.satellite.mean_table import MeanItem
from scipy.interpolate.interpnd import LinearNDInterpolator
import shelve
import pickle
from tables import File, Filters, open_file
from multiprocessing import Pool

def create_linear_interpolator(points, mean_item : MeanItem):
    interpolator = LinearNDInterpolator(points, mean_item.mean)
    interpolator_std = LinearNDInterpolator(points, np.sqrt(mean_item.variance))
    return interpolator, interpolator_std

@dataclass
class InterpolatorContainer:
    n_layers : int
    name :str
    interpolator_mean : LinearNDInterpolator
    interpolator_std : LinearNDInterpolator

def create_interpolator_from_layers(parameters):
    points, mean_items, name = parameters
    n_layers = len(mean_items)
    if n_layers == 1:
        mean_item = mean_items[0]
    else:
        mean_item = MeanItem.join_item(*mean_items)
    interpolator, interpolator_std = create_linear_interpolator(points, mean_item)
    return InterpolatorContainer(n_layers, name, interpolator, interpolator_std)

def save_interpolator(file : File, container: InterpolatorContainer):
    filters = Filters(complevel=3, fletcher32=True)
    data = pickle.dumps(container)
    array = file.create_array(file.root, container.name, obj=data, filters=filters)
    array.flush()
    return 0

def parameters_generator(points, mean_items: List[MeanItem], n = 1, particle="proton"):
    n_layrers = len(mean_items)
    for i in range(n_layrers - (n-1)):
        name = particle + "_".join(map(str,range(i, i + n)))
        temp = mean_items[i:i + n]
        yield (points, temp, name)

def save_interpolators(file, mean_items: List[MeanItem], points, n = 1, particle="proton", n_cpu_cores = None):
    with open_file(file, "a") as h5file:
        with Pool(n_cpu_cores) as p:
            n_layers = len(mean_items)
            with tqdm(total = (n_layers - (n-1))) as pbar:
                generator = parameters_generator(points, mean_items, n=n, particle=particle)
                for result in p.imap_unordered(create_interpolator_from_layers, generator):
                    save_interpolator(h5file, result)
                    pbar.update(1)
    return 0

def convert_data_to_mean_points(data):
    points = np.zeros(shape=(data.size, 3))
    points[:, 0] = data["energy"][:]
    points[:, 1] = data["theta"][:]
    points[:, 2] = data["shift"][:]
    mean_items = []
    n = data["mean"][0].size
    for i in range(n):
        temp = MeanItem(
            mean= np.abs(np.round(data["mean"][:, i], decimals=10)),
            variance=np.abs(np.round(data["variance"][:, i], decimals=10)),
            number= data["number"]
        )
        mean_items.append(temp)
    return mean_items, points





# class Filter(abc.ABC):
#     @abc.abstractmethod
#     def filter(self, event: np.ndarray, error: np.ndarray) -> bool:
#         pass
#
# class FullEnergyFilter(Filter):
#
#     def __init__(self, ):
#
#     def filter(self, event: np.ndarray, error: np.ndarray) -> bool:
#
#
#
# class SingleProcessing:
#
#     def __init__(self):
#         self.filters