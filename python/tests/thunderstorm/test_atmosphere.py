import unittest
import xml.etree.ElementTree as ET
from phd.thunderstorm.atmosphere import create_full_earth_atmosphere, save_full_atmosphere
from python.phd.thunderstorm.atmosphere import air_accuracy
import io
from lxml import etree
class AtmosphereTest(unittest.TestCase):
    def test_accuracy_air(self):
        sum_ = 0
        for k, v in air_accuracy.items():
            sum_ += v
        result = 100 - sum_
        print(result)
        self.assertAlmostEqual(0.0, result, places=4)

    def test_full_atmosphere(self):
        save_full_atmosphere("../../../cxx/thunderstorm/gdml/NeutronFullAtmosphere.gdml")
        save_full_atmosphere("../../../cxx/thunderstorm/gdml/SpaceCraftFullAtmosphere.gdml", with_spacecraft=True)