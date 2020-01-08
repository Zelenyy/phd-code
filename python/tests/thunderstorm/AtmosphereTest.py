import unittest

from python.phd.thunderstorm.atmosphere import air_accuracy


class AtmosphereTest(unittest.TestCase):
    def test_accuracy_air(self):
        sum_ = 0
        for k, v in air_accuracy.items():
            sum_ += v
        result = 100 - sum_
        print(result)
        self.assertAlmostEqual(0.0, result, places=4)
