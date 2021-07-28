from tables import open_file


path = "/mnt/storage1/data/sri-satellite/paramDetailProton/paramDetailProton.hdf5"


with open_file(path) as h5file:
    for group in h5file.root:
        table = h5file.get_node(group, "cellEnergyDeposit")
        print(repr(table.attrs))
        break


if __name__ == '__main__':
    pass
