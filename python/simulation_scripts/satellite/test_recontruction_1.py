import tables
from phd.satellite.geant4_server import DetectorMode, Geant4Server
import numpy as np
from phd.satellite.run import request_generator
from phd.satellite.satellite_pb2 import Run
from phd.satellite.single_processing import load_likelihood_factory, SingleProcessing

reconstructed_dtype = np.dtype(
[
    ("original", "d", (3,)),
    ("reconstructed", "d", (3,)),
    ("success", np.bool ),
]
)


def add_row(table, value, result):
    row = table.row
    row["original"] = np.array([value["energy"], value["theta"], value["shift"]])
    row["reconstructed"] = result.x
    row["success"] = result.success
    row.append()
    return 0

def main():
    path_mesh = "/mnt/storage2/phd/data/satellite/polistyrene/mean_mesh.hdf5"
    path_test = "/mnt/storage2/phd/data/satellite/polistyrene/mean_mesh.hdf5"
    lh_fact = load_likelihood_factory(path_mesh, particle="proton")
    single_processing = SingleProcessing(lh_fact)

    with tables.open_file(path_test) as h5file, tables.open_file("recon_1.hdf5", "w") as fout:
        table = fout.create_table(fout.root, "proton_1", description=reconstructed_dtype)

        for group in h5file.root:


    result = single_processing.process()
    add_row(table, value, result)
    table.flush()
                    # print(value["energy"], lh_fact.energy_normilizer.unnormalize(result.x[0]))

if __name__ == '__main__':
    main()