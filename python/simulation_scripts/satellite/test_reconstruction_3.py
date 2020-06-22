from multiprocessing.pool import Pool

import tables
import numpy as np
from phd.satellite.single_processing import load_likelihood_factory, load_splitting_likelihood_factory, join_event
from phd.satellite.processing.single_processing import SingleProcessing, DetectorCharacter

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
    path_mesh = "/home/zelenyy/data/satellite/mean_mesh.hdf5"
    path_test = "/home/zelenyy/data/satellite/proton_mean/proton.hdf5"
    splitting = [4 for i in range(20)]
    lh_split = load_splitting_likelihood_factory(path_mesh, particle="proton", splitting=splitting)
    detector = DetectorCharacter(aperture=30, proton_treshold=4.0, proton_high=100)
    single_processing = SingleProcessing(lh_split, detector)

    with tables.open_file(path_test) as h5file, tables.open_file("recon_3.hdf5", "w") as fout:
        table = fout.create_table(fout.root, "proton_mean_test", description=reconstructed_dtype)
        data = h5file.get_node("/", "deposit").read()
        print(data.shape)
        mean, var = join_event(data, splitting)
        print(mean.shape)
        # print(data.size)
        with Pool() as p:
            count = 0
            for result, value in zip(p.imap(single_processing.process, mean, chunksize=50), data):
                if result is not None:
                    add_row(table,value,result)
                else:
                    count+=1
        table.flush()
        print(count)

if __name__ == '__main__':
    main()