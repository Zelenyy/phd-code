import unittest

from phd.thunderstorm.thunderstorm_pb2 import CylinderIdList
from phd.utils.histogram_pb2 import Histogram2DList
from phd.utils.proto_list import ProtoList


class ProtoListTest(unittest.TestCase):

    def test_read(self):
        path = "/home/zelenyy/npm/phd/phd-code/cxx/thunderstorm/run/stacking_simple1.bin"
        with ProtoList(path, CylinderIdList) as protoList:
            item = protoList[0]
            print(item.eventId, item.cylinderId)

if __name__ == '__main__':
    unittest.main()