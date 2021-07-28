path = "/home/zelenyy/data/NICA/bmn_run2874_dst.root"

import uproot
file = uproot.open(path)
print(file.keys())
print(file["bmnroot;1"])

if __name__ == '__main__':
    pass