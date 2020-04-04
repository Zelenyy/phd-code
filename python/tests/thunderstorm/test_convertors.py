import os
from unittest import TestCase
import tables
import numpy as np
from phd.thunderstorm.convert_to_hdf5 import CylinderProtoSet, HistogramProtoSet
from phd.utils.hdf5_tools import ConverterFromBinToHDF5, ProtoSetReader
from tables import Filters


class ConvertorTest(TestCase):
    def test_proto_set_convertor(self):
        readers = [ProtoSetReader("gammaSeed.bin", CylinderProtoSet),
                   ProtoSetReader("positronSeed.bin", CylinderProtoSet),
                   ProtoSetReader("histogram.bin", HistogramProtoSet)]
        path = "/home/zelenyy/npm/phd/phd-code/cxx/thunderstorm/run"
        filters = Filters(complevel=3, fletcher32=True)
        convertor = ConverterFromBinToHDF5(readers)
        for reader in readers:
            reader.set_filters(filters)
        convertor.convert(path, "./test.hdf5")
