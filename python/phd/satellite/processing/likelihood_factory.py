from typing import List

import numpy as np
from phd.satellite.mean_table import Normilizer
from phd.satellite.processing.likelihood import Likelihood
from scipy.interpolate import RegularGridInterpolator


class LikelihoodFactory:
    def __init__(self,
                 interpolators_mean: List[RegularGridInterpolator],
                 interpolators_std: List[RegularGridInterpolator],
                 energy_normilizer: Normilizer = None,
                 theta_normilizer: Normilizer = None,
                 shift_normilizer : Normilizer = None,
                 splitting = None):
        self.interpolators_mean =  interpolators_mean
        self.interpolators_std = interpolators_std
        self.energy_normilizer = energy_normilizer
        self.theta_normilizer = theta_normilizer
        self.shift_normilizer = shift_normilizer
        self.splitting = splitting

    def build(self, event: np.ndarray):
        treshhold = 10
        indx = event > (event.mean()/treshhold)
        new_event = np.zeros(event.size)
        new_event[indx] = event[indx]
        i = np.argmin(new_event > 0)
        return Likelihood(self.interpolators_mean[:i], self.interpolators_std[:i], event[:i],
                          self.energy_normilizer, self.theta_normilizer, self.shift_normilizer,
                          splitting = self.splitting)