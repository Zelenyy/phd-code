from phd.satellite.mean_table import convert_to_mesh


def main():
    convert_to_mesh("./proton_mean/proton.hdf5", "mean_mesh.hdf5",particle="proton")
    # convert_to_mesh("./electron/electron.hdf5", "mean_mesh.hdf5",particle="electron")


if __name__ == '__main__':
    main()