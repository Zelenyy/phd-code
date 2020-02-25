import os
from dataclasses import dataclass

import numpy as np
from dataforge import MetaRepr, Meta
from tables import File, Group, Int32Col, IsDescription

from ..utils.hdf5_tools import txtDataReader, dtypeDataReader, Reader

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

CYLINDER_ID_DTYPE = np.dtype([
    ("id", "i"),
    ("parent_id", "i"),
    ("energy", "d"),
    ("theta", "d"),
    ("radius", "d"),
    ("z", "d"),
])

READERS_CYLINDER_ID_DATA = [dtypeDataReader(file, CYLINDER_ID_DTYPE) for file in
                         ['gamma.bin', 'electron.bin', 'positron.bin']]

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

@dataclass
class BinsDescription(MetaRepr):
    bins: np.ndarray
    bins_type: str
    unit: str = None

    def to_meta(self) -> 'Meta':
        return Meta(
            {
                "type": self.bins_type,
                "unit": self.unit
            }
        )

@dataclass
class HistDescription(MetaRepr):
    bins_x: BinsDescription
    bins_y: BinsDescription
    hist_type: str = None
    particle: str = None

    def to_meta(self) -> 'Meta':
        return Meta({
            "type" : self.hist_type,
            "particle" : self.particle,
            "bins" : {
                "x" : self.bins_x.to_meta(),
                "y" : self.bins_y.to_meta(),
            }
        })

@dataclass
class HistogrammsDwyer2003:
    description: HistDescription
    number_of_event: int
    data: list


class HistDwyer2003Reader(Reader):

    NUMBER_OF_HIST = 4

    def parse_description(self, fin):
        hist_desc = []
        fin.readline()
        for i in range(self.NUMBER_OF_HIST):
            fin.readline()
            hist_type = fin.readline().split()[-1]
            particle = fin.readline().split()[-1]
            bins_x_type, x_unit = fin.readline().split()[-2:]
            x_unit = x_unit[1:-1]
            bins_x = np.fromstring(fin.readline().rstrip(), "d", sep=" ")
            bins_y_type, y_unit = fin.readline().split()[-2:]
            y_unit = y_unit[1:-1]
            bins_y = np.fromstring(fin.readline().rstrip(), "d",  sep=" ")
            fin.readline()
            hist_desc.append(
                HistDescription(
                    BinsDescription(bins_x, bins_x_type, x_unit),
                    BinsDescription(bins_y, bins_y_type, y_unit),
                    hist_type,
                    particle
                )
            )
        return hist_desc

    def parse_events(self,fin):
        events = []
        while True:
            line = fin.readline()
            print(line, end="")
            if not line:
                break
            temp = []
            for i in range(self.NUMBER_OF_HIST):
                print(fin.readline(), end="")
                print(fin.readline(), end="")
                data = []
                while True:
                    line = fin.readline()
                    if line.rstrip() == "":
                        break
                    data.append(np.fromstring(line.rstrip(), "i",  sep=" "))
                data = np.array(data)
                temp.append(data)
            events.append(temp)
            print(fin.readline(), end="")
        return events

    def parse(self, path: str):
        with open(path, "r") as fin:
            hist_desc =  self.parse_description(fin)
            events = self.parse_events(fin)
        number_of_event = len(events)
        result = []
        for i in range(self.NUMBER_OF_HIST):
            hist = HistogrammsDwyer2003(
                hist_desc[i],
                number_of_event,
                data = [event[i] for event in events]
            )
            result.append(hist)
        return result




    def __call__(self, path: str, h5file: File, group: Group):
        hists = self.parse(path)
        name  =  self.filename[:self.filename.rfind('.')]
        print(name)
        hists_group = h5file.create_group(group,name)
        for hist in hists:
            name = hist.description.particle + "_" + hist.description.hist_type
            hist_gr = h5file.create_group(hists_group, name)
            for bins, name in zip(
                    [hist.description.bins_x, hist.description.bins_y],
                    ["xbins", "ybins"]
            ):
                array = h5file.create_array(hist_gr, name, obj=bins.bins)
                for key, value in bins.to_meta().to_flat().items():
                    array.attrs[key] = value
                array.flush()

            nx = hist.description.bins_x.bins.size
            ny = hist.description.bins_y.bins.size

            class HistByEvent(IsDescription):
                number = Int32Col(pos=1)
                histogramm = Int32Col(shape=(nx-1, ny-1), pos=2)

            # print(nx-1, ny-1, hist.data[0].shape)
            # print(nx-1, ny-1, hist.data[1].shape)
            table = h5file.create_table(hist_gr, "histByEvent", description=HistByEvent)
            table.append([(indx, item) for indx, item in enumerate(hist.data)])
            table.flush()




