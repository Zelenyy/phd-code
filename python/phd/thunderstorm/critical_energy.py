import logging
import os
from dataclasses import dataclass
from string import Template

import numpy as np
import tables

import pyinotify
from phd.thunderstorm.convert_to_hdf5 import CylinderProtoSet
from phd.utils.hdf5_tools import ProtoSetReader
from phd.utils.run_tools import G4CinServer, CinServerParameters
from tables import Filters, Table, Group
import matplotlib.pyplot as plt

ROOT_PATH = os.path.dirname(__file__)

INPUT_TEMPLATE = """/npm/geometry/type gdml
/npm/geometry/gdml critical_energy.gdml
/npm/thunderstorm/physics ${physics}
/npm/thunderstorm/minimal_energy ${energy} MeV
/npm/thunderstorm/stepping/type critical_energy
/npm/thunderstorm/stacking/electron false
/npm/thunderstorm/stacking/positron false
/npm/thunderstorm/stacking/gamma false
/npm/thunderstorm/stacking/save_gamma false
/npm/thunderstorm/stacking/save_electron true
/npm/thunderstorm/stacking/save_electron_cut ${energy} MeV
separator
"""

MESSEGE = """/gps/particle e-
/gps/number 1
/gps/direction 0 0 -1
/gps/ene/mono ${energy} MeV
/gps/position 0.0 0.0 0.0 m
/run/beamOn ${number}
separator
"""


class Processor:

    def init_messege(self) -> str:
        return ""

    def process(self, event):
        return None

    def accept(self, event):
        pass

def create_gdml(template_file, values: dict):
    with open(template_file) as fin:
        gdml_template = fin.read()
        gdml_template = Template(gdml_template)
    with open("critical_energy.gdml", 'w') as fout:
        fout.write(gdml_template.substitute(values))
    return 0

class CriticalEnergyProcessor(Processor):

    def __init__(self, meta):
        self.reader = ProtoSetReader("stacking_simple.bin", CylinderProtoSet)
        filters = Filters(complevel=3, fletcher32=True)
        self.reader.set_filters(filters)
        self.path_hdf5 = "result.hdf5"
        self.counter = 0
        self.mess_templte = Template(MESSEGE)
        self.meta = meta
        self.step = 0.001

    def init_messege(self) -> str:
        return self.mess_templte.substitute(self.meta)

    def process(self, event):
        group_path = self.convert(event.pathname)
        os.remove(event.pathname)
        with tables.open_file(self.path_hdf5) as h5file:
            table: Table = h5file.get_node(group_path, "stacking_simple")
            n_electron = table.nrows
            n_primary = table.attrs["number"]
            gamma = n_electron / n_primary
            if gamma > 1:
                return None
            else:
                self.meta["energy"] = self.meta["energy"] + self.step
                return self.init_messege()

    def accept(self, event):
        return event.name == "stacking_simple.bin"

    def convert(self, path):
        with tables.open_file(self.path_hdf5, mode="a") as h5file:
            group = h5file.create_group(h5file.root, "sim{}".format(str(self.counter).rjust(4, '0')))
            self.reader(path, h5file, group)
            for table in h5file.iter_nodes(group):
                if (isinstance(table, Group)):
                    continue
                for key, value in self.meta.items():
                    table.attrs[key] = value
            self.counter += 1
            return group._v_pathname


class G4CinServerHandler(pyinotify.ProcessEvent):

    def my_init(self, server: G4CinServer, processor: Processor):
        self.server = server
        self.processor = processor
        self.server.send(processor.init_messege())

    def process_IN_CREATE(self, event):
        logging.root.info(str(event))

    def process_IN_CLOSE_WRITE(self, event):
        if self.processor.accept(event):
            result = self.processor.process(event)
            if result is not None:
                self.server.send(result)
            else:
                raise KeyboardInterrupt



import star
import numpy as np
from phd.thunderstorm import atmosphere
from scipy.optimize import root_scalar

def get_critical_energy(height = 0, field = 0):
    """

    :param height: meters
    :param field:kV/cm
    :return:
    """
    material = star.electron.PredefinedMaterials.AIR_DRY_NEAR_SEA_LEVEL

    density = atmosphere.ISACalculator.density(height)  # kg/m3

    def critical_energy_equation(energy):
        data = star.electron.calculate_stopping_power(material, np.asarray([energy]))
        stopPower = data["stopping_power_total"][0]
        return field - stopPower*density
    try:
        critical_energy_root = root_scalar(
            critical_energy_equation,
            bracket=(0.001, 2.0),
                        )
    except ValueError as err:
        print(err)
        return None
    return critical_energy_root

from scipy.linalg import lstsq

# def calculate_secondary_production_rate(path):
#     bins = np.arange(-500.0, 501, 1)
#     x = bins[:-1]
#     M = x[:, np.newaxis] ** [0, 1]
#
#     dtype = np.dtype(
#         [
#             ("field", "d"),
#             ("height", "d"),
#             ("energy", "d"),
#             ("k", "d"),
#             ("b", "d")
#         ]
#     )
#
#     with tables.open_file(path) as h5file:
#         result = []
#         for group in h5file.root:
#             table = h5file.get_node(group, "stacking_simple")
#             data = table.read()
#             field = table.attrs["values_gdml_field"][0]
#             height = table.attrs["values_gdml_height"][0]
#             energy =  table.attrs["values_macros_energy"]
#             number =  table.attrs["values_macros_number"]
#             temp, _ = np.histogram(data["z"], bins=bins)
#             temp = np.cumsum(temp)
#             y = temp / number
#             p, res, rnk, s = lstsq(M, y)
#             result.append((field, height, energy, p[1], p[0]))
#         return np.array(result, dtype=dtype)

@dataclass(eq=True, frozen=True)
class FieldHeigth:
    field: float
    height : float



def get_group(path):
    with tables.open_file(path) as h5file:
        result = {}
        for group in h5file.root:
            table = h5file.get_node(group, "stacking_simple")
            field = table.attrs["values_gdml_field"][0]
            height = table.attrs["values_gdml_height"][0]
            energy =  table.attrs["values_macros_energy"]
            key = FieldHeigth(field, height)
            if key in result.keys():
                result[key].append((energy, group._v_name))
            else:
                result[key] = [(energy, group._v_name)]
        for value in result.values():
            value.sort(key=lambda x: x[0])
        return result

def plot_secondary_production_rate(path, output="plot"):

    if not os.path.exists(output):
        os.mkdir(output)

    groups = get_group(path)
    bins = np.arange(-500.0, 501, 1)
    x = bins[:-1]
    result = []
    dtype = np.dtype(
        [
            ("field", "d"),
            ("height", "d"),
            ("energy", "d"),
            ("k", "d"),
            ("b", "d")
        ]
    )

    with tables.open_file(path) as h5file:
        for key, value in groups.items():
            energy_cut = value[0][0]
            plt.clf()
            for energy, group_name in value:
                table: tables.Table = h5file.get_node("/{}".format(group_name), "stacking_simple")
                data = table.read()
                data = data[data["energy"] > energy_cut]
                number = table.attrs["values_macros_number"]
                temp, _ = np.histogram(data["z"], bins=bins)
                temp = np.cumsum(temp[::-1])
                y = temp / number
                plt.plot(x, y)
            path = os.path.join(output, "{}m_{}kV_m.png".format(key.height, key.field*1e4))
            plt.xlabel("Height, meters")
            plt.ylabel("Cumulative number of electron")
            plt.tight_layout()
            plt.savefig(path, format="png", transparent=True, dpi = 600)
    return 0


def calculate_secondary_production_rate(path, rate_cut = 0.001, method="simple"):
    if method not in ["simple", "rate-cut"]:
        logging.root.warning("Bad method for {}".format(calculate_secondary_production_rate.__name__))
    groups = get_group(path)
    bins = np.arange(-500.0, 501, 1)
    x = bins[:-1]
    M = x[:, np.newaxis] ** [0, 1]
    result = []

    dtype = np.dtype(
        [
            ("field", "d"),
            ("height", "d"),
            ("energy", "d"),
            ("k", "d"),
            ("b", "d"),
            ("chi2", "d")
        ]
    )

    with tables.open_file(path) as h5file:
        for key, value in groups.items():
            energy_cut = value[0][0]
            for energy, group_name in value:
                table: tables.Table = h5file.get_node("/{}".format(group_name), "stacking_simple")
                data = table.read()
                data = data[data["energy"] > energy_cut]
                number = table.attrs["values_macros_number"]
                temp, _ = np.histogram(data["z"], bins=bins)
                temp = np.cumsum(temp[::-1])
                y = temp / number
                p, res, rnk, s = lstsq(M, y)
                k = p[1]
                if method=="rate-cut":
                    if k<= rate_cut:
                        energy_cut = energy
                elif method=="simple":
                    energy_cut = energy

                y_fit = p[1]*x + p[0]
                indx = y_fit !=0
                y_fit = y_fit[indx]
                chi2 = np.sum(((y[indx] - y_fit)/y_fit)**2)

                result.append((key.field, key.height, energy, p[1], p[0], chi2))
        return np.array(result, dtype=dtype)


class CriticalEnergyProvider:
    def __init__(self):
        data_path = os.path.join(os.path.dirname(__file__), "data", "critical_energy.npy")
        self.data = np.load(data_path)
        self.pairs = np.unique(self.data[["height", "field"]])

    def get_critical_energy(self, height = 0.0, field = 0.0, length = 1000.0):
        indx = self.pairs["height"] == height
        if np.all(np.logical_not(indx)):
            raise Exception("Non table height")
        pairs = self.pairs[indx]
        fields = np.sort(pairs["field"])
        f_indx = (field < fields).argmax()
        field = fields[f_indx]
        indx = np.logical_and(self.data["height"] == height, self.data["field"] == field)
        rate_cut = 1/length
        data = self.data[indx]
        indx = np.argsort(data["energy"])
        data = data[indx]
        indx = data["k"] > rate_cut
        if np.any(indx):
            indx = indx.argmax()
        else:
            indx = -1
        return data["energy"][indx]