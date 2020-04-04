import logging
import abc
import logging
import os
import shutil
import struct
import sys
from typing import List, Union, Optional

import numpy as np
from dataforge import Meta
from dataforge.io import JsonMetaFormat
from numpy import dtype
from tables import open_file, File, Group, Filters

from .run_tools import InputData


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

    def __init__(self, readers: List[Reader]):
        self.readers = readers
        self.logger: logging.Logger = logging.getLogger(__name__)
        self.logger.addHandler(
            logging.FileHandler("convertor.log")
        )

    def convert(self, paths_data: Union[List, str], path_h5file: str, mode: str = "a",
                meta: Optional[Union[List[Meta], Meta]] = None) -> str:

        if isinstance(paths_data, str):
            paths_data = [paths_data]
        if meta is not None:
            if isinstance(meta, dict):
                meta = [meta]
        os.makedirs(os.path.split(path_h5file)[0], exist_ok=True)
        with open_file(path_h5file, mode=mode, title='Auto convert from binary files', ) as h5file:
            for indx, path in enumerate(paths_data):
                nameGroup = os.path.normpath(path).split(os.sep)[-1]

                if mode == 'a' or mode == 'r+':
                    nameGroup = self._check_name(h5file.root, nameGroup)

                group = h5file.create_group('/', nameGroup, title='Auto group from path {}'.format(path))

                if meta is not None:
                    meta_item = JsonMetaFormat().dump_meta(meta[indx]).encode("utf-8")
                    h5file.create_array(group, "meta", obj=meta_item).flush()

                for reader in self.readers:
                    self.logger.debug("Reader: {} {}".format(type(reader), reader.filename))
                    pathToFile = os.path.join(path, reader.filename)
                    if os.path.exists(pathToFile) and (os.path.getsize(path) != 0):
                        reader(pathToFile, h5file, group)
                for table in h5file.iter_nodes(group):
                    if (isinstance(table, Group)):
                        continue
                    logging.debug(str(table))
                    if meta is not None:
                        for key, value in meta[indx].to_flat().items():
                            if sys.getsizeof(value) > 64 * 1024:
                                continue
                            if "@" in key: continue
                            key = key.replace(".", "_")
                            table.attrs[key] = value
                    table.flush()
                    self.logger.debug(repr(table.attrs))
            h5file.close()
        return path_h5file

    _check_name_counter = 0

    def _check_name(self, root, name, postfix=''):
        if name + postfix in root:
            self._check_name_counter += 1
            return self._check_name(root, name, postfix='_re_' + str(self._check_name_counter))
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
        if ("e-" in self.tableName):
            self.tableName = self.tableName.replace("e-", "electron")
        if ("e+" in self.tableName):
            self.tableName = self.tableName.replace("e+", "positron")
        # if data.size == 0:
        #     my_table = h5file.create_table(group, self.tableName, desc **self.settings)
        # else:
        my_table = h5file.create_table(group, self.tableName, obj=data, **self.settings)
        my_table.flush()


class txtDataReader(Reader):
    def __init__(self, filename: str, **kwargs):
        self.kwargs = kwargs
        Reader.__init__(self, filename)

    def __call__(self, path: str, h5file: File, group: Group):
        data = np.loadtxt(path, **self.kwargs)
        if data.size == 1:
            data = data.reshape((1,))
        self.tableName = self.filename[:self.filename.rfind('.')]
        try:
            my_table = h5file.create_table(group, self.tableName, obj=data, **self.settings)
        except IndexError:
            print(group, self.tableName, data, data.size, type(data), data.shape)
            raise
        my_table.flush()


def get_convertor(readers: list, path_h5file, clear=False):
    filters = Filters(complevel=3, fletcher32=True)
    convertor = ConverterFromBinToHDF5(readers)
    for reader in readers:
        logging.root.debug("Reader: {} {}".format(type(reader), reader.filename))
        reader.set_filters(filters)

    def post_run_processor(input_data: InputData):
        path = input_data.path
        convertor.convert(path, path_h5file, meta=input_data.to_meta())
        if clear:
            shutil.rmtree(path)

    return post_run_processor


class ProtoReader(Reader):
    def __init__(self, filename, proto_convertor):
        self.proto_convertor = proto_convertor
        Reader.__init__(self, filename)

    def __call__(self, path: str, h5file: File, group: Group):
        self.proto_convertor(path,  h5file, group, self.settings)

class ProtoSetConvertor(abc.ABC):

    def __init__(self, h5file: File, group: Group, filename: str = None, settings = None):
        self.h5file = h5file
        self.group = group
        self.filename = filename
        self.settings = settings

    def init(self, proto_item = None):
        pass

    @abc.abstractmethod
    def convert(self, data: bytes):
        pass

class DtypeProtoSetConvertor(ProtoSetConvertor):

    dtype = None

    def __init__(self, h5file: File, group: Group, filename: str = None, settings=None):
        ProtoSetConvertor.__init__(self, h5file, group, filename, settings)
        self.init()

    def init(self, proto_item = None):
        self.tableName = self.filename
        if ("e-" in self.tableName):
            self.tableName = self.tableName.replace("e-", "electron")
        if ("e+" in self.tableName):
            self.tableName = self.tableName.replace("e+", "positron")
        my_table = self.h5file.create_table(self.group, self.tableName, description=self.dtype, **self.settings)
        my_table.flush()

class ProtoSetReader(Reader):
    def __init__(self, filename, proto_set_convertor):
        self.proto_convertor = proto_set_convertor
        Reader.__init__(self, filename)

    def __call__(self, path: str, h5file: File, group: Group):
        filename = self.filename[:self.filename.rfind('.')]
        convertor = self.proto_convertor(h5file, group, filename, self.settings)
        with open(path, "rb") as fin:
            while True:
                size = fin.read(4)
                if size == b"":
                    break
                size = struct.unpack("i", size)[0]
                data = fin.read(size)
                convertor.convert(data)