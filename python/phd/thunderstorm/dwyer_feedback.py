import numpy as np
import tables
from phd.utils.convertor_tools import direction_to_degree
from phd.utils.path_tools import find_by_meta
from scipy.optimize import curve_fit


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