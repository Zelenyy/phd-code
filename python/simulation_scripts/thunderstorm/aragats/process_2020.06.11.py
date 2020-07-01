import os
import tables
import numpy as np

bins_1 = np.arange(0.05, 1.01, 0.05)
bins_2 = np.arange(1, 202, 2)


def get_values(data):
    values_low, _ = np.histogram(data, bins=bins_1)
    values_high, _ = np.histogram(data, bins=bins_2)
    return values_high, values_low

class HistrogramData(tables.IsDescription):
    energy = tables.Float64Col()
    number = tables.Int32Col()
    bins_low = tables.Float64Col(shape=bins_1.size)
    bins_high = tables.Float64Col(shape=bins_2.size)
    gamma_hist_low = tables.Float64Col(shape=bins_1.size - 1)
    gamma_hist_high = tables.Float64Col(shape=bins_2.size -1)
    electron_hist_low = tables.Float64Col(shape=bins_1.size -1)
    electron_hist_high = tables.Float64Col(shape=bins_2.size -1)


def convert():
    paths = os.listdir(".")
    with tables.open_file("hist_220_expacs.hd5", "w") as fout:
        hist_table = fout.create_table("/", "histogram", description=HistrogramData)
        for path in paths:
            path = os.path.join(path, "result.hdf5")
            with tables.open_file(path) as h5file :
                for group in h5file.root:
                    table = h5file.get_node(group, "particle_detector")
                    energy = table.attrs["values_energy"]
                    number = table.attrs["values_number"]
                    data = table.read()
                    indx_gamma = data["particle"] == 22
                    gamma = data[indx_gamma]
                    g_hig, g_low = get_values(gamma["energy"])
                    e_hig, e_low = get_values(data[np.logical_not(indx_gamma)]["energy"])
                    row = hist_table.row
                    row["energy"] = energy
                    row["number"] = number
                    row["bins_low"] = bins_1
                    row["bins_high"] = bins_2
                    row["gamma_hist_low"] = g_low
                    row["gamma_hist_high"] = g_hig
                    row["electron_hist_low"] = e_low
                    row["electron_hist_high"] = e_hig
                    row.append()
                hist_table.flush()
    return 0

def add_1mev():
    with tables.open_file("hist_220_expacs.hd5", "a") as fout:
        hist_table = fout.get_node("/", "histogram")
        row = hist_table.row
        path = "electron_1MeV.hdf5"
        with tables.open_file(path) as h5file:
            for group in h5file.root:
                table = h5file.get_node(group, "particle_detector")
                field = table.attrs["values_field_z"]
                phys = table.attrs["values_physics"]
                if field == 220.0 and phys=="standard_opt_4":
                    energy = table.attrs["values_energy"]
                    number = table.attrs["values_number"]
                    data = table.read()
                    indx_gamma = data["particle"] == 22
                    gamma = data[indx_gamma]
                    g_hig, g_low = get_values(gamma["energy"])
                    e_hig, e_low = get_values(data[np.logical_not(indx_gamma)]["energy"])
                    row["energy"] = energy
                    row["number"] = number
                    row["bins_low"] = bins_1
                    row["bins_high"] = bins_2
                    row["gamma_hist_low"] = g_low
                    row["gamma_hist_high"] = g_hig
                    row["electron_hist_low"] = e_low
                    row["electron_hist_high"] = e_hig
                    row.append()
                    break
        hist_table.flush()

def process():
    with tables.open_file("hist_220_expacs.hd5") as fin:
        hist_table = fin.get_node("/", "histogram")
        data = hist_table.read()
        print(np.sort(data["energy"]))


import argparse
def main():
    parser = argparse.ArgumentParser(description='Process aragats EXAPCS.')
    parser.add_argument('--convert', action='store_true')
    parser.add_argument('--add', action='store_true')
    parser.add_argument('--process', action='store_true')

    args = parser.parse_args()

    if args.convert:
        convert()
    elif args.process:
        process()
    elif args.add:
        add_1mev()

if __name__ == '__main__':
    main()