from unittest import TestCase

import numpy as np
from phd.thunderstorm.dwyer_feedback import get_bins_from_center, load_reversed_data


class Dwyer2003(TestCase):
    def test_get_bins_from_center(self):
        a = np.arange(0,91, 10)
        b = get_bins_from_center(a)
        print(b)
        self.assertAlmostEqual(b[1], 5.0)

    def test_load_revesed(self):
        path = "/mnt/storage2/phd/data/thunderstorm/reversed_1/grid_v2.hdf5"
        height = 0.0
        field = 6e-4
        theta, energy, probability = load_reversed_data(path, field, height)
        print(theta)