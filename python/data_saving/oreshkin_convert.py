import json
import os

import numpy as np
import tables

names_stack = [    'energy',
        'theta',
        'radius',
        'z']
dtype_stack = [(name, 'd') for name in names_stack]


def convert_data(path, output):
    dirlis = os.listdir(path)
    with tables.open_file(output, "w") as h5file:
        data = []
        for it in dirlis:
            path_dir = os.path.join(path, it)
            if (os.path.isdir(path_dir)):
                data.append(
                    np.fromfile(os.path.join(path_dir, "data", "number.bin"), dtype="i")
                )
        data = np.hstack(data)
        array = h5file.create_array("/", "number", obj=data)
        array.flush()

        data = []
        for it in dirlis:
            path_dir = os.path.join(path, it)
            if (os.path.isdir(path_dir)):
                data.append(
                    np.fromfile(os.path.join(path_dir, "data", "electron.bin"), dtype=dtype_stack)
                )
        data = np.hstack(data)

        table = h5file.create_table("/", "electron", obj=data)
        table.flush()

        path_meta = os.path.join(path_dir,"meta.json")
        with open(path_meta, "rb") as fin:
            meta = json.load(fin)
        for key, value in meta.items():
            table.attrs[key] = value
        table.flush()
    return 0

def main():
    path = "/home/zelenyy/data/thunderstorm/temp_oreshkin/positron_700_100/"
    convert_data(path, "positron.hdf5")
    path = "/home/zelenyy/data/thunderstorm/temp_oreshkin/no_positron_800_100/"
    convert_data(path, "no_positron.hdf5")

if __name__ == '__main__':
        main()