import struct
from collections import Sized
from typing import Type


class ProtoList(Sized):
    BUFF_SIZE = 8
    def __init__(self, path, topClass: Type):
        self.path = path
        self.topClass = topClass

    def __enter__(self):
        self.fin = open(self.path, "rb")
        buff = self.fin.read(self.BUFF_SIZE)
        if buff == b"": raise Exception("Empty file")
        size = struct.unpack("q", buff)[0]
        self.messege_sizes = [size]
        self.coords = [0]
        coord = 0
        while True:
            coord += self.BUFF_SIZE + size
            self.fin.seek(coord)
            buff = self.fin.read(self.BUFF_SIZE)
            if buff == b"": break
            self.coords.append(coord)
            size = struct.unpack("q", buff)[0]
            self.messege_sizes.append(size)

        return self

    def __len__(self):
        return len(self.messege_sizes)

    def __getitem__(self, item):
        self.fin.seek(self.coords[item] + self.BUFF_SIZE)
        data = self.fin.read(self.messege_sizes[item])
        protoClass = self.topClass()
        protoClass.ParseFromString(data)
        return protoClass

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.fin.close()

