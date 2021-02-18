import os

import numpy as np
from phd.utils.hdf5_tools import ProtoSetConvertor, DtypeProtoSetConvertor
from phd.utils.histogram_pb2 import Histogram2DList
from tables import Int32Col, IsDescription, File, Group

from phd.thunderstorm.thunderstorm_pb2 import CylinderIdList, ParticleDetectorList, Cumulator1D, Cumulator2D
from ..utils.hdf5_tools import txtDataReader, dtypeDataReader

names_txt = ['Primary', 'Gamma', 'Electron', 'Positron']
dtype_txt = np.dtype([(name, 'i') for name in names_txt])

READERS_TXT = [txtDataReader('number.txt', dtype=dtype_txt)]

names_cylinder = ['energy',
                  'theta',
                  'radius',
                  'z']

dtype_cylinder = np.dtype([(name, 'd') for name in names_cylinder])

READERS_CYLINDER_DATA = [dtypeDataReader(file, dtype_cylinder) for file in
                         ['gamma.bin', 'electron.bin', 'positron.bin']]

# CYLINDER_ID_DTYPE = np.dtype([
#     ("id", "i"),
#     ("parent_id", "i"),
#     ("energy", "d"),
#     ("theta", "d"),
#     ("radius", "d"),
#     ("z", "d"),
# ])

# READERS_CYLINDER_ID_DATA = [dtypeDataReader(file, CYLINDER_ID_DTYPE) for file in
#                          ['gamma.bin', 'electron.bin', 'positron.bin']]

TREE_SOCKET_DTYPE = np.dtype([
    ("id", "i"),
    ("parent_id", "i"),
    ("particle", "i"),
    ("zero", "i"),
    ("energy", "d"),
    ("theta", "d"),
    ("radius", "d"),
    ("z", "d"),
])

READER_TREE_SOCKET_DATA = [dtypeDataReader("TreeTracking.bin", TREE_SOCKET_DTYPE)]

def convert_name(name):
    if name == "e-":
        return "electron"
    elif name == "e+":
        return "positron"
    else:
        return name

def get_named_number_readers(path, prefix):
    txtFileName = prefix + "_number.txt"
    txtFile = os.path.join(path, txtFileName)
    with open(txtFile) as fin:
        header = fin.readline()
    particles = header.split()
    names = map(convert_name, particles)
    dtype_txt = np.dtype([(name, 'i') for name in names])
    READERS_TXT = [txtDataReader(txtFileName, dtype=dtype_txt, skiprows=1)]
    return particles, READERS_TXT


def get_named_cylinder_readers(particles, prefix):
    readers_cylinder_data = [dtypeDataReader(prefix + "_" + name + ".bin", dtype_cylinder)
                             for name in particles]
    return readers_cylinder_data

def get_named_number_readers_1(particles, prefix):
    txtFileName = prefix + "_number.txt"
    names = map(convert_name, particles)
    dtype_txt = np.dtype([(name, 'i') for name in names])
    readers_txt = [txtDataReader(txtFileName, dtype=dtype_txt, skiprows=1)]
    return readers_txt

CYLINDER_ID_DTYPE = np.dtype([
    ("event", "i"),
    ("id", "i"),
    ("parent_id", "i"),
    ("particle", "i"),
    ("energy", "d"),
    ("theta", "d"),
    ("radius", "d"),
    ("z", "d"),
    ("time", "d")
])



class CylinderProtoSet(DtypeProtoSetConvertor):
    dtype = CYLINDER_ID_DTYPE

    def convert(self, data: bytes):
        cylinder_id_list = CylinderIdList()
        cylinder_id_list.ParseFromString(data)
        n = len(cylinder_id_list.cylinderId)
        data = np.zeros(n, dtype=self.dtype)
        data["event"] = cylinder_id_list.eventId
        for indx, item in enumerate(cylinder_id_list.cylinderId):
            data["id"][indx] = item.id
            data["parent_id"][indx] = item.parent_id
            data["particle"][indx] = item.particle
            data["energy"][indx] = item.energy
            data["theta"][indx] = item.theta
            data["radius"][indx] = item.radius
            data["z"][indx] = item.z
            data["time"][indx] = item.time
        table = self.h5file.get_node(self.group, self.tableName)
        table.append(data)
        table.flush()

PARTICLE_DETECTOR_DTYPE = np.dtype([
    ("event", "i"),
    ("particle", "i"),
    ("energy", "d"),
    ("theta", "d"),
    ("radius", "d"),
    ("time", "d"),
])

class ParticleDetectorProtoSet(DtypeProtoSetConvertor):
    dtype = PARTICLE_DETECTOR_DTYPE

    def convert(self, data: bytes):
        proto_list = ParticleDetectorList()
        proto_list.ParseFromString(data)
        n = len(proto_list.data)
        data = np.zeros(n, dtype=self.dtype)
        data["event"] = proto_list.eventId
        for indx, item in enumerate(proto_list.data):
            data["particle"][indx] = item.particle
            data["energy"][indx] = item.energy
            data["theta"][indx] = item.theta
            data["radius"][indx] = item.radius
            data["time"][indx] = item.time
        table = self.h5file.get_node(self.group, self.tableName)
        table.append(data)
        table.flush()

class Cumulator1DProtoSet(ProtoSetConvertor):

    eventID = 0

    def __init__(self, h5file: File, group: Group, filename: str = None, settings=None):
        ProtoSetConvertor.__init__(self, h5file, group, filename, settings)
        self.init()

    def init(self, proto_item=None):
        self.groupName = self.filename
        if ("e-" in self.groupName):
            self.groupName = self.groupName.replace("e-", "electron")
        if ("e+" in self.groupName):
            self.groupName = self.groupName.replace("e+", "positron")
        self.h5file.create_group(self.group, self.groupName)

    def convert(self, data: bytes):
        proto = Cumulator1D()
        proto.ParseFromString(data)
        data = np.array(proto.data)
        data_group = self.h5file.get_node(self.group, self.groupName)
        array = self.h5file.create_array(data_group, "event"+str(self.eventID).rjust(5, "0"), obj=data)
        array.attrs["eventID"] = self.eventID
        array.attrs["number"] =proto.number
        array.attrs["left"] = proto.left
        array.attrs['right'] = proto.right
        array.flush()
        self.eventID+=1

class Cumulator2DProtoSet(ProtoSetConvertor):

    eventID = 0

    def __init__(self, h5file: File, group: Group, filename: str = None, settings=None):
        ProtoSetConvertor.__init__(self, h5file, group, filename, settings)
        self.init()

    def init(self, proto_item=None):
        self.groupName = self.filename
        if ("e-" in self.groupName):
            self.groupName = self.groupName.replace("e-", "electron")
        if ("e+" in self.groupName):
            self.groupName = self.groupName.replace("e+", "positron")
        self.h5file.create_group(self.group, self.groupName)

    def convert(self, data: bytes):
        proto = Cumulator2D()
        proto.ParseFromString(data)
        data = np.array(proto.data)
        data_group = self.h5file.get_node(self.group, self.groupName)
        array = self.h5file.create_array(data_group, "event"+str(self.eventID).rjust(5, "0"), obj=data)
        array.attrs["eventID"] = self.eventID


        def fill_bins(bins, prefix):
            array.attrs[prefix + "_number"] = bins.number
            array.attrs[prefix + "_left"] = bins.left
            array.attrs[prefix + "_right"] = bins.right

        fill_bins(proto.x, "x")
        fill_bins(proto.y, "y")

        array.flush()
        self.eventID+=1


class HistogramProtoSet(ProtoSetConvertor):
    first = True
    event = 0
    def init(self, proto_item = None):
        hists_group = self.h5file.create_group(self.group, "histogram")
        for hist in proto_item.histogram:
            meta = {}
            for item in hist.meta:
                meta[item.key] = item.value
            name = meta["particle"] + "_" + meta["type"]
            hist_gr = self.h5file.create_group(hists_group, name)
            nx = len(hist.xbins.bins)
            ny = len(hist.ybins.bins)

            for bins, name in zip(
                    [hist.xbins, hist.ybins],
                    ["xbins", "ybins"]
            ):
                array = self.h5file.create_array(hist_gr, name, obj=np.array(bins.bins))
                for key, value in meta.items():
                    array.attrs[key] = value
                array.flush()

            class HistByEvent(IsDescription):
                event = Int32Col(pos=1)
                histogram = Int32Col(shape=(nx - 1, ny - 1), pos=2)

            table = self.h5file.create_table(hist_gr, "histByEvent", description=HistByEvent, **self.settings)
            for key, value in meta.items():
                table.attrs[key] = value
            table.flush()

    def convert(self, data :bytes):
        histList = Histogram2DList()
        histList.ParseFromString(data)
        if self.first:
            self.init(histList)
            self.first = False
        hists_group = self.h5file.get_node(self.group, "histogram")
        for hist in histList.histogram:
            meta = {}
            for item in hist.meta:
                meta[item.key] = item.value
            name = meta["particle"] + "_" + meta["type"]
            hist_gr = self.h5file.get_node(hists_group, name)
            table = self.h5file.get_node(hist_gr, "histByEvent")
            row = table.row
            row["event"] = self.event
            self.event+=1
            nx = self.h5file.get_node(hist_gr, "xbins").nrows
            ny = self.h5file.get_node(hist_gr, "ybins").nrows
            data = np.array(hist.data).reshape((nx - 1, ny - 1))
            row["histogram"] = data
            row.append()
            table.flush()

