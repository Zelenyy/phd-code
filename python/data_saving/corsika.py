import numpy as np
from tables import open_file

HDF5_FILE = "../../data/thunderstorm/CORSIKA.hdf5"


def corsika_1():
    """
    Data from CORSIKA simulation.
    See: https://docs.google.com/document/d/1Xz6_jGrjm6_B9LeMMkhXD1fsEMJMzkZKKqzPzrksMB0/edit
    """
    field = [0., 0.1, 0.5, 1., 1.3, 1.5, 1.6, 1.7, 1.8]
    number_of_gamma = [293923., 295647., 306962., 338228.,
                       383402., 444727., 496979., 582886., 754262.]
    number_of_electrons = [17696, 17432, 18513, 20836,
                           24300, 28476, 31682, 36851, 45562]
    data = [(i,j,k) for i,j,k in zip(field, number_of_electrons, number_of_gamma)]
    data = np.array(data,
                    dtype = [("field", 'd'), ("electrons", 'd'), ("gamma", 'd')])
    with open_file(HDF5_FILE, mode="a") as h5file:
        table = h5file.create_table(h5file.root, "corsika1", obj=data)
        table.attrs["primary"] = "gamma"
        table.attrs["number_of_primaries"] = 1_000_000
        table.attrs["primary_min_energy_mev"] = 1
        table.attrs["primary_max_energy_mev"] = 100
        table.attrs["primary_law"] = "power"
        table.attrs["primary_law_index"] = -1.42
        table.attrs["observation_level_meter"] = 3200
        table.attrs["field_low_level_meter"] = 3250
        table.attrs["field_high_level_meter"] = 4250
        table.attrs["cloud_thikness_gr_cm2"] = 88.89

        table.flush()
    return True

if __name__ == '__main__':
    corsika_1()


