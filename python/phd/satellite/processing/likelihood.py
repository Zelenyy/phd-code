from typing import List

import numpy as np
from phd.satellite.mean_table import Normilizer
from scipy.interpolate import RegularGridInterpolator
from scipy.stats import multivariate_normal


class Likelihood:
    def __init__(self,
                 interpolators_mean: List[RegularGridInterpolator],
                 interpolators_std: List[RegularGridInterpolator],
                 event: np.ndarray,
                 energy_normilizer: Normilizer = None,
                 theta_normilizer: Normilizer = None,
                 shift_normilizer: Normilizer = None,
                 splitting = None,
                 radius=0.015  # meter
                 ):
        self.mean_list = interpolators_mean
        self.std_list = interpolators_std
        self.energy_normilizer = energy_normilizer
        self.theta_normilizer = theta_normilizer
        self.shift_normilizer = shift_normilizer
        self.splitting = splitting
        self.radius = radius
        self.event = event
        self.full_energy = self._full_energy(event)
        self.min_range = self._full_range(event)/1000.0 #meter
        self.max_theta = self._max_theta(radius, self.min_range)

        # print("Max theta: {}".format(self.max_theta))

    def _full_energy(self, event):
        return self.energy_normilizer.normalize(np.sum(event))

    def _full_range(self, event):
        i = len(event)
        if self.splitting is None:
            return i
        else:
            return np.sum(self.splitting[:i-1])

    def _max_theta(self, radius, deep):
        if self.splitting is None:
            if deep == self.splitting[0]:
                return 1.0
        else:
            if deep == 0:
                return 1.0
        max_theta = np.arctan(2*radius / deep)
        return self.theta_normilizer.normalize(max_theta)

    def chi2(self, point):
        mean = np.array([inter(point) for inter in self.mean_list])
        std = np.array([inter(point) for inter in self.std_list])
        return np.sum((self.event - mean)**2/std), len(self.event)

    def pdf(self, point):
        mean = np.array([inter(point) for inter in self.mean_list])
        std = np.array([inter(point) for inter in self.std_list])
        cov = np.round(np.square(std), 5)
        return multivariate_normal.pdf(self.event, mean=mean, cov=cov)

    def max_plus_theta(self, shift):
        theta_plus_max = np.rad2deg(np.arctan((self.radius + shift) / self.min_range))
        theta_plus_max = self.theta_normilizer.normalize(theta_plus_max)
        if theta_plus_max > 1.0:
            return 1.0
        elif theta_plus_max < 0.0:
            return 0.0
        return theta_plus_max

    def max_minus_theta(self, shift):
        theta_minus_max = -np.rad2deg(np.arctan((2 * self.radius - shift) / self.min_range))
        theta_minus_max =  self.theta_normilizer.normalize(theta_minus_max)
        if theta_minus_max > 1:
            return 1.0
        elif theta_minus_max < 0:
            return 0.0
        return theta_minus_max

    def __call__(self, point):
        if np.any(point < 0) or np.any(point > 1):
            # print("Boundary")
            return 0.0
        if self.full_energy - 0.1 > point[0]:
            # print("energy")
            return 0.0
        shift = self.shift_normilizer.unnormalize(point[2])
        theta_minus_max = self.max_minus_theta(shift)
        theta_plus_max = self.max_plus_theta(shift)
        if point[1] > theta_plus_max or point[1] < theta_minus_max:
            # print(theta_minus_max, theta_plus_max)
            # print("Theta")
            return 0.0
        mean = np.array([inter(point)[0] for inter in self.mean_list])
        std = np.array([inter(point)[0] for inter in self.std_list])
        cov = np.round(np.square(std), 5)
        self.last_cov = cov
        if np.any(cov == 0.0):
            # print("Cov")
            return 0.0

        sum_ = multivariate_normal.logpdf(self.event, mean=mean, cov=cov)
        if sum_ == 0:
            return 0.0
        return 1 / sum_

    def many_points(self, points):
        size = points.shape[0]
        result = np.zeros(size, "d")

        indx_1 = np.any(points >= 0, axis=1)
        indx_2 = np.any(points <= 1, axis=1)
        indx_3 = self.full_energy <= points[:, 0]
        indx_4 = self.max_theta >= points[:, 1]
        indx = np.logical_and(np.logical_and(np.logical_and(indx_1, indx_2), indx_3), indx_4)
        mean = np.array([inter(points[indx]) for inter in self.mean_list]).T
        std = np.array([inter(points[indx]) for inter in self.std_list]).T
        cov = np.round(np.square(std), 5)

        indx_cov = np.any(cov == 0.0, axis=1)
        self.last_cov = cov
        print(mean.shape)
        print(indx.size)
        n = mean.shape[0]
        sum_ = np.zeros(n)
        for i in range(n):
            try:
                if not indx_cov[i]:
                    sum_[i] = multivariate_normal.logpdf(self.event, mean=mean[i], cov=cov[i])
            except Exception:
                print(i)
                print(mean[i])
                print(cov[i])

        result[indx] = 1 / sum_
        return result