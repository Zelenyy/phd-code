import json
import logging
import os
import sys
from typing import List, Union
import abc
import numpy as np
from numpy import dtype
from tables import open_file, File, Group, Filters


class Reader(abc.ABC):
    def __init__(self, filename: str):
        self.filename = filename
        self.settings = {}

    @abc.abstractmethod
    def __call__(self, path: str, h5file: File, group: Group):
        pass

    def set_filters(self, filters: Filters):
        self.settings['filters'] = filters
        return self


class ConverterFromBinToHDF5:

    def __init__(self, readerList: List[Reader]):
        self.readerList = readerList

    def convert(self, pathsBin: Union[List, str], pathHDF5: str, mode: str = "a") -> str:

        if isinstance(pathsBin, str):
            pathsBin = [pathsBin]
        os.makedirs(os.path.split(pathHDF5)[0], exist_ok=True)
        with open_file(pathHDF5, mode=mode, title='Auto convert from binary files', ) as h5file:
            for path in pathsBin:
                with open(os.path.join(path, 'meta.json')) as fin:
                    meta = json.load(fin)
                nameGroup = os.path.normpath(path).split(os.sep)[-1]

                if mode == 'a' or mode == 'r+':
                    nameGroup = self._check_name(h5file.root, nameGroup)

                group = h5file.create_group('/', nameGroup, title='Auto group from path {}'.format(path))
                for reader in self.readerList:
                    pathToFile = os.path.join(path, 'data', reader.filename)
                    reader(pathToFile, h5file, group)
                for table in h5file.iter_nodes(group):
                    logging.debug(str(table))
                    for key, value in meta.items():
                        if sys.getsizeof(value) > 64 * 1024:
                            continue
                        table.attrs[key] = value
                    table.flush()
                    logging.debug(repr(table.attrs))
            h5file.close()
        return pathHDF5

    _check_name_counter = 0

    def _check_name(self, root, name, postfix=''):
        if name + postfix in root:
            self._check_name_counter += 1
            return self._check_name(root, name, postfix='(' + str(self._check_name_counter) + ')')
        else:
            self._check_name_counter = 0
            return name + postfix


class dtypeDataReader(Reader):
    def __init__(self, filename: str, dt: dtype):
        self.dtype = dt
        Reader.__init__(self, filename)

    def __call__(self, path: str, h5file: File, group: Group):
        data = np.fromfile(path, dtype=self.dtype)
        self.tableName = self.filename[:self.filename.rfind('.')]
        my_table = h5file.create_table(group, self.tableName, obj=data, **self.settings)
        my_table.flush()


class txtDataReader(Reader):
    def __init__(self, filename: str, **kwargs):
        self.kwargs = kwargs
        Reader.__init__(self, filename)

    def __call__(self, path: str, h5file: File, group: Group):
        data = np.loadtxt(path, **self.kwargs)
        self.tableName = self.filename[:self.filename.rfind('.')]
        my_table = h5file.create_table(group, self.tableName, obj=data, **self.settings)
        my_table.flush()
