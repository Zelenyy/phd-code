from unittest import TestCase

import numpy as np
from phd.thunderstorm.dwyer_feedback import get_bins_from_center


class Test(TestCase):
    def test_get_bins_from_center(self):
        a = np.arange(0,91, 10)
        b = get_bins_from_center(a)
        print(b)
        self.assertAlmostEqual(b[1], 5.0)