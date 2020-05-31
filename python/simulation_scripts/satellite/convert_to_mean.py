from phd.satellite.mean_table import convert_to_mesh


def main():
    convert_to_mesh("proton.hdf5", "mean_mesh.hdf5",particle="proton")


if __name__ == '__main__':
    main()