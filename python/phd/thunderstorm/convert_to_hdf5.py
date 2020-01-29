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
