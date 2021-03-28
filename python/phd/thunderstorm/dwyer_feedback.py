import json
import os
from dataclasses import dataclass

import numpy as np
import tables
from dataforge import Meta, Name
from phd.utils.convertor_tools import direction_to_degree
from phd.utils.path_tools import find_by_meta, get_attrs_values
from scipy.optimize import curve_fit
from tables import File, Group, pickle


def load_reversed_data(path, field=8e-4, height=0):
    paths = find_by_meta(path,target_node="particle_detector_number", values_gdml_fieldValueZ=field, values_gdml_height = height)
    probability = []
    energy = []
    theta = []
    with tables.open_file(path) as h5file:
        for path_node in paths:
            table = h5file.get_node(path_node)
            data = table.read()
            indx = data['electron'] == 0
            probability.append(np.sum(indx)/table.attrs["values_macros_number"])
            energy.append(table.attrs["values_macros_energy"])
            theta.append(direction_to_degree(table.attrs["values_macros_direction"]))
    return np.array(theta), np.array(energy), np.array(probability)


def load_reversed_energy(path, field=8e-4, height=0):
    paths = find_by_meta(path,target_node="particle_detector_electron", values_gdml_fieldValueZ=field, values_gdml_height = height)
    result = []
    energy = []
    theta = []
    with tables.open_file(path) as h5file:
        for path_node in paths:
            table = h5file.get_node(path_node)
            data = table.read()
            result.append(data)
            energy.append(table.attrs["values_macros_energy"])
            theta.append(direction_to_degree(table.attrs["values_macros_direction"]))
    return np.array(theta), np.array(energy), result

def get_bins_from_center(center: np.ndarray)-> np.ndarray:
    bins = np.zeros(center.size+1, dtype=center.dtype)
    diff = np.diff(center)/2
    bins[1:-1] = center[:-1] + diff
    bins[0] = center[0] - diff[0]
    bins[-1] = center[-1] + diff[-1]
    return bins


class ReverseHistogramm:
    def __init__(self, bins, probability):
        """
        :param bins: (theta, energy) center of histogram bin
        :param probability: probability of successful electron reverse
        """
        self.probability = probability
        self.theta_center = bins[0]
        self.energy_center = bins[1]
        self.theta_bins = get_bins_from_center(self.theta_center)
        self.energy_bins = get_bins_from_center(self.energy_center)

    def calculate_reverse_ET(self, data: np.ndarray):
        result, _ , _ = np.histogram2d(np.rad2deg(data["theta"]), data["energy"], bins=(self.theta_bins, self.energy_bins))
        return result*self.probability



def histogrammed_reverse_data(path1, path2, field, height=0):
    theta1, energy1, probability1 = load_reversed_data(path1, field, height)
    theta2, energy2, probability2 = load_reversed_data(path2, field, height)
    theta = np.hstack((theta1, theta2))
    energy = np.hstack((energy1, energy2))
    probability = np.hstack((probability1, probability2))
    xi = np.sort(np.unique(theta))
    yi = np.sort(np.unique(energy))
    histogramm = np.zeros((xi.size, yi.size))
    for t, e, p in zip(theta, energy, probability):
        indx = np.where(xi == t)[0][0]
        indy = np.where(yi == e)[0][0]
        histogramm[indx, indy] = p
    return ReverseHistogramm((xi,yi), 1 - histogramm)

def histogrammed_reverse_data_v2(path, field, height=0):
    theta, energy, probability = load_reversed_data(path, field, height)
    xi = np.sort(np.unique(theta))
    yi = np.sort(np.unique(energy))
    histogramm = np.zeros((xi.size, yi.size))
    for t, e, p in zip(theta, energy, probability):
        indx = np.where(xi == t)[0][0]
        indy = np.where(yi == e)[0][0]
        histogramm[indx, indy] = p
    return ReverseHistogramm((xi,yi), 1 - histogramm)

def count_feedback(data, reverseHist: ReverseHistogramm):
    result = reverseHist.calculate_reverse_ET(data)
    return result


def get_dwyer_line():
    path = "/mnt/storage2/phd/data/thunderstorm/Graph/Dwyer2003.dat"
    with open(path) as fin:
        text = fin.read()
        text = text.replace(",", ".")
        text = text.split("\n")
        data = [[float(i) for i in line.split()] for line in text[:-1]]
    data = np.array(data)

    def power(x, alpha, const, back):
        return const * np.exp(-x / alpha) + back

    popt, _ = curve_fit(power, xdata=np.log(data[:, 0]), ydata=np.log(data[:, 1]), p0=[100, 2000, 500])
    return lambda x :  np.exp(power(np.log(x), *popt))

@dataclass
class FeedBack:
    height: float
    field : float
    cell : float
    gamma: float
    positron: float
    gamma_err: float
    positron_err: float
    number_of_seed: float


class FeedBackCalculator:

    def __init__(self, reverseHist: ReverseHistogramm):
        self.reverseHist = reverseHist

    def calculate(self, data):
        if data.size != 0:
            result = self.reverseHist.calculate_reverse_ET(data)
            return result.sum()
        else:
            return 0

    def calculate_from_node(self, h5file: File, group: Group):
        table_gamma = h5file.get_node(group, "gamma")
        number = table_gamma.attrs["values_macros_number"]
        height = table_gamma.attrs["values_gdml_height"]
        cell = table_gamma.attrs["values_gdml_cellHeight"]
        field = table_gamma.attrs["values_gdml_fieldValueZ"]

        table_positron = h5file.get_node(group, "positron")
        pos_fb = self.calculate(table_positron.read())
        gamma_fb = self.calculate(table_gamma.read())
        gamma_err = (gamma_fb ** 0.5) / number
        pos_err = (pos_fb ** 0.5) / number
        gamma_fb = gamma_fb / number
        pos_fb = pos_fb / number
        return FeedBack(height, field, cell, gamma_fb, pos_fb, gamma_err, pos_err, number)

def find_group_by_meta(filename, **kwargs):
    result = []
    with tables.open_file(filename) as h5file:
        for group in h5file.root:
            meta = h5file.get_node(group, "meta").read()
            meta = Meta(json.loads(meta))
            flag = True
            for key, value in kwargs.items():
                name = Name.from_string(key.replace("_", "."))
                if meta[name] != value:
                    flag = False
                    break
            if flag:
                result.append(group._v_pathname)
    return result

def calc_fb(path, fbCalc: FeedBackCalculator, field):
    result = []
    print("field", field)
    paths = find_group_by_meta(path, values_gdml_fieldValueZ=field)
    print(paths)
    with tables.open_file(path) as h5file:
        for p in paths:
            result.append(fbCalc.calculate_from_node(h5file, p))
    return result


def get_feedback_dwyer2003():
    path_reverse = "/mnt/storage2/phd/data/thunderstorm/reversed_1/grid_v2.hdf5"
    fields = get_attrs_values(path_reverse, "values_gdml_fieldValueZ")
    fields = sorted(list(fields))
    result = []
    path = "/mnt/storage2/phd/data/thunderstorm/dwyer2003/"
    for field in fields:
        reverseHist = histogrammed_reverse_data_v2(path_reverse, field)
        fbCalc = FeedBackCalculator(reverseHist)
        for i in range(6):
            path_hdf5 = os.path.join(path, f"diff_{i}.hdf5")
            result += calc_fb(path_hdf5, fbCalc, field)

    result_1 = []
    path = "/mnt/storage2/phd/data/thunderstorm/dwyer2003/"
    reverseHist = histogrammed_reverse_data_v2(path_reverse, 5e-4)
    fbCalc = FeedBackCalculator(reverseHist)
    for i in range(6):
        path_hdf5 = os.path.join(path, f"diff_{i}.hdf5")
        result_1 += calc_fb(path_hdf5, fbCalc, 4e-4)
    for i in range(6):
        path_hdf5 = os.path.join(path, f"diff_{i}.hdf5")
        result_1 += calc_fb(path_hdf5, fbCalc, 4.5e-4)

    reverseHist = histogrammed_reverse_data_v2(path_reverse, fields[2])
    fbCalc = FeedBackCalculator(reverseHist)
    for i in range(6):
        path_hdf5 = os.path.join(path, f"diff_{i}.hdf5")
        result_1 += calc_fb(path_hdf5, fbCalc, 6e-4)

    summary_result = []
    for field in fields:
        temp = []
        for item in result:
            if item.field == field:
                temp.append(item)
        if temp != []:
            gamma_tot = sum([it.number_of_seed * it.gamma for it in temp]) / sum([it.number_of_seed for it in temp])
            pos_tot = sum([it.number_of_seed * it.positron for it in temp]) / sum([it.number_of_seed for it in temp])
            gamma_err = (sum([it.number_of_seed * it.gamma for it in temp]) ** 0.5) / sum(
                [it.number_of_seed for it in temp])
            pos_err = (sum([it.number_of_seed * it.positron for it in temp]) ** 0.5) / sum(
                [it.number_of_seed for it in temp])
            summary_result.append(
                FeedBack(
                    temp[0].height,
                    field,
                    temp[0].cell,
                    gamma_tot,
                    pos_tot,
                    gamma_err,
                    pos_err,
                    sum([it.number_of_seed for it in temp])
                )
            )

    summary_result.append(result_1[0])
    summary_result.append(result_1[1])

    summary_result_6 = []

    temp = result_1[2:]
    gamma_tot = sum([it.number_of_seed * it.gamma for it in temp]) / sum([it.number_of_seed for it in temp])
    pos_tot = sum([it.number_of_seed * it.positron for it in temp]) / sum([it.number_of_seed for it in temp])
    gamma_err = (sum([it.number_of_seed * it.gamma for it in temp]) ** 0.5) / sum(
        [it.number_of_seed for it in temp])
    pos_err = (sum([it.number_of_seed * it.positron for it in temp]) ** 0.5) / sum(
        [it.number_of_seed for it in temp])
    summary_result_6.append(
        FeedBack(
            temp[0].height,
            6e-4,
            temp[0].cell,
            gamma_tot,
            pos_tot,
            gamma_err,
            pos_err,
            sum([it.number_of_seed for it in temp])
        )
    )
    summary_result.append(summary_result_6[0])
    with open("sum_feed_back_phd.obj", "wb") as fout:
        pickle.dump(summary_result, fout)

    return summary_result