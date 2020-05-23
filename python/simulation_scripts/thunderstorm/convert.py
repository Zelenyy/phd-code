import argparse
import shutil
import sys

from phd.thunderstorm.convert_to_hdf5 import CylinderProtoSet, HistogramProtoSet
from phd.utils.hdf5_tools import ProtoSetReader, ConverterFromBinToHDF5
from tables import Filters


def get_convertor(readers: list):
    filters = Filters(complevel=3, fletcher32=True)
    convertor = ConverterFromBinToHDF5(readers)
    for reader in readers:
        reader.set_filters(filters)
    return convertor

def create_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("--clear", action="store_true")
    parser.add_argument("-o", "--output", default="convert.hdf5")
    parser.add_argument("-i","--input", required=True)
    return parser


def main():
    args = create_parser().parse_args()
    path = args.input

    readers = [
        ProtoSetReader("treeTracking.bin", CylinderProtoSet),
        ProtoSetReader("gammaSeed.bin", CylinderProtoSet),
        ProtoSetReader("positronSeed.bin", CylinderProtoSet),
        ProtoSetReader("histogram.bin", HistogramProtoSet)]

    convertor = get_convertor(readers)
    convertor.convert(path, args.output)
    if args.clear:
        shutil.rmtree(path)

if __name__ == '__main__':
    main()