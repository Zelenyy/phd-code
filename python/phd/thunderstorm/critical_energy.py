import logging
import os
from string import Template

import numpy as np
import tables

import pyinotify
from phd.thunderstorm.convert_to_hdf5 import CylinderProtoSet
from phd.utils.hdf5_tools import ProtoSetReader
from phd.utils.run_tools import G4CinServer, CinServerParameters
from tables import Filters, Table, Group

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





