import os
import unittest

from phd.utils.gdml import GDML, box


class GDMLTest(unittest.TestCase):

    def test_gdml(self):
        gdml = GDML() \
            .add_solid(box("box", 1, 1, 1)).build()

        print(gdml)

if __name__ == '__main__':
    unittest.main()
