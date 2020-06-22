import tables
from phd.satellite.processing.likelihood_factory import LikelihoodFactory
from scipy.interpolate import RegularGridInterpolator
import numpy as np
from phd.satellite.mean_table import MeanItem, Normilizer


#
# def create_linear_interpolator(points, mean_item : MeanItem):
#     interpolator = LinearNDInterpolator(points, mean_item.mean)
#     interpolator_std = LinearNDInterpolator(points, np.sqrt(mean_item.variance))
#     return interpolator, interpolator_std
#
# @dataclass
# class InterpolatorContainer:
#     n_layers : int
#     name :str
#     interpolator_mean : LinearNDInterpolator
#     interpolator_std : LinearNDInterpolator
#
#
# def create_interpolator_from_layers(parameters):
#     points, mean_items, name = parameters
#     n_layers = len(mean_items)
#     if n_layers == 1:
#         mean_item = mean_items[0]
#     else:
#         mean_item = MeanItem.join_item(*mean_items)
#     interpolator, interpolator_std = create_linear_interpolator(points, mean_item)
#     return InterpolatorContainer(n_layers, name, interpolator, interpolator_std)
#
# def save_interpolator(file : File, container: InterpolatorContainer):
#     filters = Filters(complevel=3, fletcher32=True)
#     data = pickle.dumps(container)
#     array = file.create_array(file.root, container.name, obj=data, filters=filters)
#     array.flush()
#     return 0

# def parameters_generator(points, mean_items: List[MeanItem], n = 1, particle="proton"):
#     n_layrers = len(mean_items)
#     for i in range(n_layrers - (n-1)):
#         name = particle + "_".join(map(str,range(i, i + n)))
#         temp = mean_items[i:i + n]
#         yield (points, temp, name)

# def save_interpolators(file, mean_items: List[MeanItem], points, n = 1, particle="proton", n_cpu_cores = None):
#     with open_file(file, "a") as h5file:
#         with Pool(n_cpu_cores) as p:
#             n_layers = len(mean_items)
#             with tqdm(total = (n_layers - (n-1))) as pbar:
#                 generator = parameters_generator(points, mean_items, n=n, particle=particle)
#                 for result in p.imap_unordered(create_interpolator_from_layers, generator):
#                     save_interpolator(h5file, result)
#                     pbar.update(1)
#     return 0

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

class NormilizerContainer:
    def __init__(self, path, particle="proton"):
        with tables.open_file(path) as h5file:
            group = tables.Group(h5file.root, particle)
            energy_node = h5file.get_node(group, "energy")
            self.energy_normilizer = Normilizer.load_normilizer(energy_node)
            theta_node = h5file.get_node(group, "theta")
            self.theta_normilizer = Normilizer.load_normilizer(theta_node)
            shift_node =  h5file.get_node(group, "shift")
            self.shift_normilizer = Normilizer.load_normilizer(shift_node)

class DataMeshLoader:
    def __init__(self, path, particle="proton"):
        with tables.open_file(path) as h5file:
            group = tables.Group(h5file.root, particle)
            self.mean = h5file.get_node(group, "mean").read()
            self.var = h5file.get_node(group, "variance").read()
            energy_node = h5file.get_node(group, "energy")
            self.energy = energy_node.read()
            self.energy_normilizer = Normilizer.load_normilizer(energy_node)
            theta_node = h5file.get_node(group, "theta")
            self.theta_normilizer = Normilizer.load_normilizer(theta_node)
            self.theta = theta_node.read()
            shift_node =  h5file.get_node(group, "shift")
            self.shift_normilizer = Normilizer.load_normilizer(shift_node)
            self.shift = shift_node.read()


def load_likelihood_factory(path, particle="proton"):
    data_mesh_loader = DataMeshLoader(path, particle=particle)
    inter_list, inter_std_list = calculate_interpolators(
        data_mesh_loader.energy,
        data_mesh_loader.theta,
        data_mesh_loader.shift,
        data_mesh_loader.mean,
        data_mesh_loader.var)
    return LikelihoodFactory(inter_list, inter_std_list,
                             data_mesh_loader.energy_normilizer,
                             data_mesh_loader.theta_normilizer,
                             data_mesh_loader.shift_normilizer)


def join_event(data, splitting):
    mean = data["mean"]
    var = data["variance"]
    i = 0
    n = len(splitting)
    m = data.size
    split_mean = np.zeros((m,n))
    split_var = np.zeros((m,n))
    for indx, split in enumerate(splitting):
        temp = []
        for j in range(split):
            temp.append(MeanItem(mean[:, i+j], var[:, i + j], number=1))
        temp = MeanItem.sum_item(*temp)
        split_mean[:, indx] = temp.mean
        split_var[: ,indx] = temp.variance
        i += split
    return split_mean, split_var

def load_splitting_likelihood_factory(path, particle="proton", splitting=None):
    if splitting is None:
        return load_likelihood_factory(path, particle)
    data_mesh_loader = DataMeshLoader(path, particle=particle)

    n = len(splitting)

    split_mean_mesh = np.zeros((n,) + data_mesh_loader.mean.shape[1:])
    split_var_mesh = np.zeros((n,) + data_mesh_loader.var.shape[1:])

    i = 0
    for indx, split in enumerate(splitting):
        temp = []
        for mean, var in zip(data_mesh_loader.mean[i:i + split], data_mesh_loader.var[i:i + split]):
            # FIXME for simulation with diff `number`
            temp.append(MeanItem(mean, var, number=1))
        temp = MeanItem.sum_item(*temp)
        split_mean_mesh[indx] = temp.mean  # /split
        split_var_mesh[indx] = temp.variance  # /(split**2)
        i += split
    inter_list, inter_std_list = calculate_interpolators(
        data_mesh_loader.energy,
        data_mesh_loader.theta,
        data_mesh_loader.shift,
        split_mean_mesh,
        split_var_mesh)

    lh_fact = LikelihoodFactory(inter_list, inter_std_list,
                                data_mesh_loader.energy_normilizer,
                                data_mesh_loader.theta_normilizer,
                                data_mesh_loader.shift_normilizer,
                                splitting)
    return lh_fact