import os
from dataclasses import dataclass
from unittest import TestCase
import tables
import numpy as np
from phd.thunderstorm.convert_to_hdf5 import CylinderProtoSet, HistogramProtoSet, Cumulator1DProtoSet, \
    Cumulator2DProtoSet
from phd.utils.hdf5_tools import ConverterFromBinToHDF5, ProtoSetReader
from tables import Filters
import matplotlib.pyplot as plt

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

    def test_cumulator(self):
        if os.path.exists("test_cumulator.hdf5"):
            os.remove("test_cumulator.hdf5")
        path = "/home/zelenyy/data/thunderstorm/test"
        readers = [
            ProtoSetReader("electron_z_cumulator.bin", Cumulator1DProtoSet),
            ProtoSetReader("electron_time_cumulator.bin", Cumulator1DProtoSet)
        ]
        filters = Filters(complevel=3, fletcher32=True)
        convertor = ConverterFromBinToHDF5(readers)
        for reader in readers:
            reader.set_filters(filters)
        convertor.convert(path, "./test_cumulator.hdf5")

        with tables.open_file("test_cumulator.hdf5") as h5file:
            for i in range(10):
                name = "event"+str(i).rjust(5, "0")
                # data = h5file.get_node("/test/electron_z_cumulator", name)
                data = h5file.get_node("/test/electron_time_cumulator", name)
                plt.plot(data)
                plt.show()

    def test_cumulator2D(self):
        if os.path.exists("test_cumulator2d.hdf5"):
            os.remove("test_cumulator2d.hdf5")
        path = "/home/zelenyy/npm/phd/phd-code/cxx/thunderstorm/run"
        readers = [
            ProtoSetReader("electron_deposit_cumulator2d.bin", Cumulator2DProtoSet),
            ProtoSetReader("electron_number_cumulator2d.bin", Cumulator2DProtoSet)
        ]
        filters = Filters(complevel=3, fletcher32=True)
        convertor = ConverterFromBinToHDF5(readers)
        for reader in readers:
            reader.set_filters(filters)
        convertor.convert(path, "./test_cumulator2d.hdf5")

        # with tables.open_file("test_cumulator2d.hdf5") as h5file:
        #     for i in range(10):
        #         name = "event"+str(i).rjust(5, "0")
        #         # data = h5file.get_node("/test/electron_z_cumulator", name)
        #         data = h5file.get_node("/test/electron_number_cumulator2d", name)
        #         plt.plot(data)
        #         plt.show()

    def test_plot_cumulator2d(self):


        @dataclass
        class UniformBins:
            number : int
            left: float
            right : float

        @dataclass
        class Cumulator2D:
            x_bins : UniformBins
            y_bins : UniformBins
            data : np.ndarray


        def load_data2d(h5file, group, name):
            node = h5file.get_node(group, name)
            x_bins = UniformBins(node.attrs["x_number"], node.attrs["x_left"],node.attrs["x_right"])
            y_bins = UniformBins(node.attrs["y_number"], node.attrs["y_left"],node.attrs["y_right"])

            data = np.asarray(node).reshape((y_bins.number, x_bins.number))
            return Cumulator2D(x_bins, y_bins, data)

        with tables.open_file("test_cumulator2d.hdf5") as h5file:
            for i in range(10):
                name = "event"+str(i).rjust(5, "0")
                cum2d = load_data2d(h5file, "/run/electron_deposit_cumulator2d", name)
                # print(cum2d)
                # break
                plt.matshow(cum2d.data[:, :500])
                plt.show()