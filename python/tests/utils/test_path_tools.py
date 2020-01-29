import unittest
import numpy as np
import tables
from phd.utils.path_tools import find_by_meta

class PathToolsTest(unittest.TestCase):

    def test_hdf5(self):
        path = "/home/zelenyy/temp/phd-run/result.hdf5"
        with tables.open_file(path) as h5file:
            table = h5file.get_node("/sim0001", "electron")
            print(repr(table.attrs))
            data = table.read()
            print(data.dtype.names)
            print(data[0])
            print(np.histogram(data["energy"]))
            print(np.histogram(data["z"]))

    def test_find_by_meta(self):
        path = "/mnt/storage2/phd/data/thunderstorm/diffModels/result_1_1e4.hdf5"
        find_by_meta(path, fieldValueZ=0.0)


if __name__ == '__main__':
    unittest.main()