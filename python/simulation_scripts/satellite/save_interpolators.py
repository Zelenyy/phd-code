import tables
from phd.satellite.single_processing import convert_data_to_mean_points, save_interpolators


def main():
    path = "proton.hdf5"
    with tables.open_file(path) as h5file:
        data = h5file.get_node("/", "deposit").read()

    mean_items, points = convert_data_to_mean_points(data)
    file = "proton_inter_1.hdf5"
    save_interpolators(file, mean_items, points, n=1, particle="proton")
    return 0

if __name__ == '__main__':
    main()