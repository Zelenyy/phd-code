from phd.satellite.satellite_pb2 import Run

import matplotlib.pyplot as plt



run = Run()

def read(file):
    with open(file, "rb") as fin:
        run.ParseFromString(fin.read())
    for event in run.event:
        plt.plot(event.deposit)
        print(event.deposit._values)
    plt.show()

if __name__ == '__main__':

    read("/home/zelenyy/npm/phd/phd-code/cxx/satellite/run/deposit.proto.bin")