from multiprocessing.pool import Pool

import tables
import numpy as np
from phd.satellite.single_processing import load_likelihood_factory
from phd.satellite.processing.single_processing import SingleProcessing

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
    path_test = "/mnt/storage2/phd/data/satellite/polistyrene/proton.hdf5"
    lh_fact = load_likelihood_factory(path_mesh, particle="proton")
    single_processing = SingleProcessing(lh_fact)

    with tables.open_file(path_test) as h5file, tables.open_file("recon_3.hdf5", "w") as fout:
        table = fout.create_table(fout.root, "proton_mean_test", description=reconstructed_dtype)
        data = h5file.get_node("/", "deposit").read()

        # m = 141
        # n = 91
        # k = 16
        # mn = m*n
        #
        # data = np.vstack((data[:mn], data[5*mn:6*mn], data[12*mn:13*mn]))
        # print(data.size)
        # indx = (data["energy"]%2 == 0)
        # indx_1 = (data["theta"]%5 == 0)
        # data = data[np.logical_and(indx, indx_1)]
        # print(data.size)
        with Pool() as p:
            count = 0
            for result, value in zip(p.imap(single_processing.process, data["mean"], chunksize=50), data):
                if result is not None:
                    add_row(table,value,result)
                else:
                    count+=1
        table.flush()
        print(count)

if __name__ == '__main__':
    main()