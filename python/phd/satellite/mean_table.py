import os
import shutil
from dataclasses import dataclass
import logging
import numpy as np
import tables
from phd.satellite.satellite_pb2 import MeanRun
from phd.utils.run_tools import InputData
from tables import open_file, Filters, File

logger = logging.getLogger(__name__)

class MeanTable:

    def __init__(self, path, event_size = 100):
        self.path = path

        self.dtype = np.dtype(
            [
                ("energy", "d"),
                ("theta", "d"),
                ("shift", "d"),
                ("mean", "d", (event_size,)),
                ("variance", "d", (event_size,)),
                ("number", "i")
                # ("theta_unit", "U6")
            ]
        )
    def init_table(self):
        filters = Filters(complevel=3, fletcher32=True)
        table = self.file.create_table(self.file.root, "deposit", description=self.dtype, filters=filters)
        return table

    def append_from_simulation(self, path):

        if self.table is None:
            self.table = self.init_table()

        logger.info("Start conversion from {}".format(path))
        sim_file = open_file(path)
        n = sim_file.root._v_nchildren
        result = np.zeros(n, dtype=self.dtype)
        for indx, group in enumerate(sim_file.root):
            table = sim_file.get_node(group, "deposit")
            data = table.read()
            result[indx]["mean"] = np.mean(data["event"])
            result[indx]["variance"] = np.var(data["event"])
            result[indx]["energy"] = table.attrs["values_macros_energy"]
            result[indx]["theta"] = table.attrs["values_macros_theta"]
            result[indx]["shift"] = table.attrs["values_macros_shift"]
            # result[indx]["theta_unit"] = table.attrs["values_macros_theta_unit"]
            result[indx]["number"] = table.attrs["values_macros_number"]
        self.table.append(result)
        self.table.flush()
        logger.info("End conversion from {}".format(path))

    def append_from_mean_run(self, run: MeanRun, meta):
        if self.table is None:
            self.table = self.init_table()
        row = self.table.row
        row["mean"] = run.mean
        row["variance"] = run.variance
        row["energy"] = meta["energy"]
        row["theta"] = meta["theta"]
        row["shift"] = meta["shift"]
        row["number"] = run.number
        row.append()
        self.table.flush()

    def append_from_input_data(self, input_data : InputData):
        path = os.path.join(input_data.path, "deposit.proto.bin")
        if self.table is None:
            self.table = self.init_table()
        run = MeanRun()
        logger.info("Open file: {}".format(path))
        with open(path, "rb") as fin:
            run.ParseFromString(fin.read())
        meta = input_data.to_meta().to_flat()
        row = self.table.row
        row["mean"] = run.mean
        row["variance"] = run.variance
        row["energy"] = meta["values.macros.energy"]
        row["theta"] = meta["values.macros.theta"]
        row["shift"] = meta["values.macros.shift"]
        # row["theta_unit"] = table.attrs["values_macros_theta_unit"]
        row["number"] = run.number
        row.append()
        self.table.flush()
        if self.clear:
            shutil.rmtree(input_data.path)

    def __enter__(self):
        path = self.path
        if not os.path.exists(path):
            self.file = open_file(path, mode="w", title="Mean energy deposit")
            self.table = None
        else:
            self.file = open_file(path, mode="a")
            self.table = self.file.get_node(self.file.root, "deposit")
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.file.close()

    @staticmethod
    def collect_mean(path_hdf5, *args):
        with MeanTable(path_hdf5) as mean_table:
            for path in args:
                mean_table.append_from_simulation(path)
        return path_hdf5

@dataclass
class MeanItem:
    mean : np.ndarray
    variance : np.ndarray
    number :int


    @staticmethod
    def join_item(*args: "MeanItem"):

        item_0 = args[0]
        mean = item_0.number*item_0.mean
        n_sum = item_0.number
        for item in args[1:]:
            mean += item.number*item.mean
            n_sum += item.number
        mean /= n_sum
        var = item_0.number*item_0.variance
        for item in args[1:]:
            var += item.number*item.variance
        # var = item_0.number*item_0.variance + mean**2 - item_0.mean**2 + 2*(item_0.mean - mean)*item_0.number*item_0.mean
        # for item in args[1:]:
        #     var += item.number*item.variance
        #     var += mean**2 - item.mean**2
        #     var += 2*(item.mean - mean)*item.number*item.mean
        var /= n_sum
        return MeanItem(mean, var, n_sum)


class Normilizer:
    def __init__(self, init=0.0, step=1.0, norm=1.0):
        self.norm = norm
        self.init = init
        self.step = step

    def normalize(self, data):
        return (data - self.init) / self.norm

    def unnormalize(self, data):
        return data*self.norm + self.init

    def index_of_non_normed(self, data):
        data = self.normalize(data)
        result: np.ndarray = np.rint(data / self.step)
        return result.astype("i")

    @staticmethod
    def get_with_array(data):
        data = np.sort(np.unique(data))
        norm = data.max() - data.min()
        normalizer = Normilizer(init=data.min(), norm=norm, step=1.0 / (data.size - 1))
        return normalizer, normalizer.normalize(data)

def convert_to_mesh(input_file : str, output: str, particle="proton"):
    with tables.open_file(input_file) as h5file:
        data = h5file.get_node("/", "deposit").read()
    norms, normeds, indexes = [], [], []
    names = ["energy","theta","shift"]
    for name in names:
        norm, normed = Normilizer.get_with_array(data[name][:])
        indx = norm.index_of_non_normed(data[name][:])
        indexes.append(indx)
        normeds.append(normed)
        norms.append(norm)

    size = data["mean"][0].size
    mean_mesh = np.zeros(shape=(size, normeds[0].size, normeds[1].size, normeds[2].size), dtype="d")
    var_mesh = np.zeros(shape=(size, normeds[0].size, normeds[1].size, normeds[2].size), dtype="d")

    for i in range(size):
        mean_mesh[i, indexes[0], indexes[1], indexes[2]] = data["mean"][:,i]
        var_mesh[i, indexes[0],indexes[1], indexes[2]] = data["variance"][:,i]

    with tables.open_file(output, "a") as h5file:
        group = h5file.create_group(h5file.root, particle)
        array = h5file.create_array(group, "mean", obj=mean_mesh)
        array.flush()
        array = h5file.create_array(group, "variance", obj=var_mesh)
        array.flush()
        for i, name in enumerate(names):
            array = h5file.create_array(group, name, obj=normeds[i])
            norm = norms[i]
            array.attrs["init"] = norm.init
            array.attrs["norm"] = norm.norm
            array.attrs["step"] = norm.step
            array.flush()

