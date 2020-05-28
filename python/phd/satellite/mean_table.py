import os
import shutil
from dataclasses import dataclass
import logging
import numpy as np
from phd.satellite.satellite_pb2 import MeanRun
from phd.utils.run_tools import InputData
from tables import open_file, Filters

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
        var = item_0.number*item_0.variance + mean**2 - item_0.mean**2 + 2*(item_0.mean - mean)*item_0.number*item_0.mean
        for item in args[1:]:
            var += item.number*item.variance
            var += mean**2 - item.mean**2
            var += 2*(item.mean - mean)*item.number*item.mean
        var /= n_sum
        return MeanItem(mean, var, n_sum)