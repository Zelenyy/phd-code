import unittest
from phd.utils.histogram_pb2 import Histogram2DList
from phd.utils.proto_list import ProtoList


class ProtoListTest(unittest.TestCase):

    def test_read(self):
        path = "/home/zelenyy/npm/phd/phd-code/cxx/thunderstorm/run/histogram4.bin"
        with ProtoList(path, Histogram2DList) as protoList:
            print(protoList[1])

if __name__ == '__main__':
    unittest.main()