import os

import numpy as np

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
    names = map(convert_name, particles)
    readers_cylinder_data = [dtypeDataReader(prefix + "_" + name + ".bin", dtype_cylinder)
                             for name in names]
    return readers_cylinder_data

def get_named_number_readers_1(particles, prefix):
    txtFileName = prefix + "_number.txt"
    names = map(convert_name, particles)
    dtype_txt = np.dtype([(name, 'i') for name in names])
    readers_txt = [txtDataReader(txtFileName, dtype=dtype_txt, skiprows=1)]
    return readers_txt