import argparse
import logging

import tables
import json
#import zlib
import numpy as np


class PytablesEncoder(json.JSONEncoder):

    def convert_attrs(self, node: tables.Leaf):
        system = {}
        for name in node.attrs._v_attrnamessys:
            system[name] = node.attrs[name]
        user = {}
        for name in node.attrs._v_attrnamesuser:
            item  = node.attrs[name]
            if str(item.__class__.__name__) == "int32":
                item = int(item)
            user[name] = item
        return {
            "system" : system,
            "user" : user
        }


    def convert_array(self, array: tables.Array):
        data = array.read()
        return {"data" : data.tolist()}

    def convert_table(self, table : tables.Table):
        data = table.read()
        return {
            "data" : {
                name : data[name].tolist() for name in data.dtype.names
            }
        }

    def default(self, obj):
        if isinstance(obj, tables.Leaf):
            attrs = self.convert_attrs(obj)
            result = {
                "attributes" : attrs
            }
            if isinstance(obj, tables.Table):
                data = self.convert_table(obj)
            elif isinstance(obj, tables.Array):
                data = self.convert_array(obj)
            result.update(data)
            return result
        elif isinstance(obj, np.signedinteger):
            return int(obj)
        else:
            return "Not impletented"


def get_nodes(h5file: tables.File, group : tables.Group) -> dict:
    result = {}
    logging.debug("Walk group: {}".format(group))
    for leaf in h5file.iter_nodes(group, classname="Leaf"):
        logging.debug("Add leaf: {}".format(leaf.name))
        result[leaf.name] = leaf
    for group in h5file.iter_nodes(group, classname="Group"):
        result[group._v_name] = get_nodes(h5file, group)

    return result

def convert_hdf5_to_json(input_file, output_file, zip=False):
    result = {}
    with tables.open_file(input_file) as h5file:
        result = {
            "title" : h5file.title
        }
        result.update(get_nodes(h5file, h5file.root))
        with open(output_file, "w") as fout:
            json.dump(result, fout, cls=PytablesEncoder)


def main():
    parser = argparse.ArgumentParser(description='Convert HDF5 file to json.\n Requires: `pip install pytables`\n Using in-memory json dumper, use for small files')
    parser.add_argument("-i", '--input',
                        help='path to input hdf5 file',
                        default="input.hdf5",
                        metavar="FILENAME")
    parser.add_argument('-o', '--output',
                        metavar="FILENAME",
                        default="from_hdf5.json",
                        help='name of output json file')
    parser.add_argument("--debug", default=False, action="store_true", help="enable debug mode")
    #parser.add_argument("--zip", default=False, action="stroe_true", help="enable zip compression")

    args = parser.parse_args()
    if args.debug:
        logging.root.setLevel(logging.DEBUG)
    convert_hdf5_to_json(args.input, args.output) #, args.zip)

if __name__ == '__main__':
    main()