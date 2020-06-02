import abc
from dataclasses import dataclass
from typing import List

import tables
from scipy.interpolate import RegularGridInterpolator
from scipy.optimize import minimize
from tqdm import tqdm
import numpy as np
from phd.satellite.mean_table import MeanItem, Normilizer
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

from scipy.stats import norm, multivariate_normal


class LikelihoodFactory:
    def __init__(self,
                 interpolators_mean: List[RegularGridInterpolator],
                 interpolators_std: List[RegularGridInterpolator],
                 energy_normilizer: Normilizer = None):
        self.interpolators_mean =  interpolators_mean
        self.interpolators_std = interpolators_std
        self.energy_normilizer = energy_normilizer

    def build(self, event: np.ndarray):
        indx = event != 0
        i = indx.argmin()
        return Likelihood(self.interpolators_mean[:i], self.interpolators_std[:i], event[:i], self.energy_normilizer)

class Likelihood:
    def __init__(self,
                 interpolators_mean: List[RegularGridInterpolator],
                 interpolators_std: List[RegularGridInterpolator],
                 event: np.ndarray,
                 energy_normilizer: Normilizer = None):
        self.mean_list = interpolators_mean
        self.std_list = interpolators_std
        self.energy_normilizer = energy_normilizer
        self.event = event
        self.full_energy = energy_normilizer.normalize(np.sum(event))

    def pdf(self, point):
        mean = np.array([inter(point) for inter in self.mean_list])
        std = np.array([inter(point) for inter in self.std_list])
        cov = np.round(np.square(std), 5)
        return multivariate_normal.pdf(self.event, mean=mean, cov=cov)

    def __call__(self, point):
        if np.any(point < 0) or np.any(point > 1):
            return 0.0
        if self.full_energy > point[0]:
            return 0.0
        mean = np.array([inter(point)[0] for inter in self.mean_list])
        std = np.array([inter(point)[0] for inter in self.std_list])
        cov = np.round(np.square(std), 5)
        if np.any(cov == 0.0):
            return 0.0
        self.last_cov = cov
        sum_ = multivariate_normal.logpdf(self.event, mean=mean, cov=cov)
        return abs(1 / sum_)

    # def calculate_many(self, points):
    #     # points = points[points[:,0] > self.full_energy]
    #     mean = np.array([inter(points) for inter in self.mean_list])
    #     std = np.array([inter(points) for inter in self.std_list])
    #     self.last_mean = mean
    #     self.last_std = std
    #     n = points.shape[0]
    #     result = np.zeros(n, "d")
    #     for i in range(n):
    #
    #         mean_t = mean[:, i]
    #         std_t = std[:, i]
    #         if np.any(std_t == 0):
    #             result[i] = -np.inf
    #         elif points[i, 0] < self.full_energy:
    #             result[i] = -np.inf
    #         else:
    #             cov = np.square(std_t)
    #             result[i] = multivariate_normal.logpdf(self.event, mean=mean_t, cov=cov)
    #     return result


class SingleProcessing:

    def __init__(self, likelihood_fact: LikelihoodFactory):
        self.likelihood_fact = likelihood_fact

    def process(self, event: np.ndarray, error: np.ndarray = None):
        likelyhood = self.likelihood_fact.build(event)
        bounds = ((likelyhood.full_energy, 1.0), (0.0, 1.0), (0.0, 1.0))
        x0 = np.array([likelyhood.full_energy, 0.5, 0.5])
        result = minimize(likelyhood, x0, method="L-BFGS-B", bounds=bounds)
        return result

def calculate_interpolators(energy, theta, shift, mean, var):
    inter_list = []
    inter_std_list = []
    n = mean.shape[0]
    for i in range(n):
        grid_inter = RegularGridInterpolator((energy, theta, shift), mean[i])
        grid_inter_std = RegularGridInterpolator((energy, theta, shift), np.sqrt(np.abs(var[i])))
        inter_list.append(grid_inter)
        inter_std_list.append(grid_inter_std)
    return inter_list, inter_std_list

def load_likelihood_factory(path, particle="proton"):
    with tables.open_file(path) as h5file:
        group = tables.Group(h5file.root, particle)
        mean = h5file.get_node(group, "mean").read()
        var = h5file.get_node(group, "variance").read()
        energy_node = h5file.get_node(group, "energy")
        energy = energy_node.read()
        energy_normilizer = Normilizer(energy_node.attrs["init"], step=energy_node.attrs["step"],
                                       norm=energy_node.attrs["norm"])
        theta = h5file.get_node(group, "theta").read()
        shift = h5file.get_node(group, "shift").read()

    inter_list, inter_std_list = calculate_interpolators(energy, theta, shift, mean, var)
    return LikelihoodFactory(inter_list, inter_std_list, energy_normilizer)

