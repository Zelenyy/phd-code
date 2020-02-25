import os
from unittest import TestCase
import tables
import numpy as np
from phd.thunderstorm.convert_to_hdf5 import HistDwyer2003Reader
from phd.utils.hdf5_tools import ConverterFromBinToHDF5


class ConvertorTest(TestCase):
    def test_dwyer2003_conver(self):
        reader = HistDwyer2003Reader("histDwyer2003.txt")
        with tables.open_file("./test.hdf5", "w") as h5file:
            reader("./histDwyer2003.txt", h5file, h5file.root)

