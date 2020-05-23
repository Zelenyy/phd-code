import struct
from typing import Type


class ProtoList:
    def __init__(self, path, topClass: Type):
        self.path = path
        self.topClass = topClass

    def __enter__(self):
        self.fin = open(self.path, "rb")
        buff = self.fin.read(4)
        if buff == b"": raise Exception("Empty file")
        size = struct.unpack("i", buff)[0]
        self.messege_sizes = [size]
        self.coords = [0]
        coord = 0
        while True:
            coord = 4 + size
            self.fin.seek(coord)
            self.coords.append(coord)
            buff = self.fin.read(4)
            if buff == b"": break
            size = struct.unpack("i", buff)[0]
            self.messege_sizes.append(size)

        return self

    def __getitem__(self, item):
        self.fin.seek(self.coords[item] + 4)
        data = self.fin.read(self.messege_sizes[item])
        protoClass = self.topClass()
        protoClass.ParseFromString(data)
        return protoClass

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.fin.close()