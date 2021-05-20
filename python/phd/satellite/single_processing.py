from dataclasses import dataclass

import tables
from scipy.interpolate import RegularGridInterpolator
import numpy as np
from phd.satellite.mean_table import MeanItem, NormilizerContainer


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



@dataclass
class DataMeshLoader:
    mean: np.ndarray
    var: np.ndarray
    energy: np.ndarray
    theta: np.ndarray
    shift: np.ndarray
    normalizers: NormilizerContainer

    @staticmethod
    def load(path, particle="proton"):
        with tables.open_file(path) as h5file:
            group = tables.Group(h5file.root, particle)
            energy_node = h5file.get_node(group, "energy")
            energy = energy_node.read()
            theta_node = h5file.get_node(group, "theta")
            theta = theta_node.read()
            shift_node = h5file.get_node(group, "shift")
            shift = shift_node.read()
            return DataMeshLoader(
                h5file.get_node(group, "mean").read(),
                h5file.get_node(group, "variance").read(),
                energy, theta, shift,
                NormilizerContainer(h5file, group)
            )

    def get_normed_axis(self):
        m, n, k = self.energy.size, self.theta.size, self.shift.size
        return np.tile(self.energy, n * k), np.tile(np.repeat(self.theta, m), k), np.repeat(self.shift, m * n)

    def get_normed_points(self):
        energy, theta, shift = self.get_normed_axis()
        points = np.zeros((energy.size, 3), dtype="d")
        points[:, 0] = energy
        points[:, 1] = theta
        points[:, 2] = shift
        return points

    def get_real_axis(self):
        m, n, k = self.energy.size, self.theta.size, self.shift.size
        enery = self.normalizers.energy_normilizer.unnormalize(self.energy)
        theta = self.normalizers.theta_normilizer.unnormalize(self.theta)
        shift = self.normalizers.shift_normilizer.unnormalize(self.shift)
        return np.tile(enery, n * k), np.tile(np.repeat(theta, m), k), np.repeat(shift, m * n)

    def get_real_points(self):
        energy, theta, shift = self.get_axis()
        points = np.zeros((energy.size, 3), dtype="d")
        points[:, 0] = energy
        points[:, 1] = theta
        points[:, 2] = shift
        return points

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

