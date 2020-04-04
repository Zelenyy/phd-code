import logging
import os
from string import Template

import numpy as np
from dataforge import Meta
from phd.satellite.covert_to_hdf5 import convert_satellite_proto
from phd.thunderstorm.convert_to_hdf5 import READERS_TXT, READERS_CYLINDER_DATA, READERS_CYLINDER_ID_DATA, \
    READER_TREE_SOCKET_DATA, HistDwyer2003Reader
from phd.utils.hdf5_tools import get_convertor, ProtoReader
from phd.utils.run_tools import multirun_command, \
    create_one_file, dir_name_generator, values_from_dict, InputData

ROOT_PATH = os.path.dirname(__file__)

INPUT_TEMPLATE = """/df/project test
/df/gdml ../../satellite.gdml
/satellite/output file
/satellite/detector ${mode}

/gps/particle ${particle}
/gps/number 1
/gps/direction ${dirX} 0.0 ${dirZ}
/gps/ene/mono ${energy} MeV
/gps/position ${posX} 0. ${posZ} m
/run/beamOn ${number}
"""



def input_generator_satellite(meta: Meta, macros_template: str):
    macros_template = Template(macros_template)
    for path, values in zip(
            dir_name_generator(".", "sim"),
            values_from_dict(meta["macros"])
    ):
        theta = values["theta"]
        radius = values["radius"]
        theta = np.deg2rad(theta)
        posX = radius*np.sin(theta)
        posZ = radius*np.cos(theta)
        dirX = np.sin(theta)
        dirZ = -np.cos(theta)
        values["posX"] = posX
        values["posZ"] = posZ
        values["dirX"] = dirX
        values["dirZ"] = dirZ
        text = macros_template.substitute(values)
        input_data_meta = {
            "macros": values,
        }
        data = InputData(
            text=text,
            path=path,
            values=Meta(input_data_meta)
        )
        yield data

def main():
    logging.basicConfig(filename="run.log")
    logging.root.setLevel(logging.DEBUG)

    values_macros = {
        "mode" : "single",
        "radius" : 0.15,
        "shift" : 0,
        "theta" : np.arange(0.0,31.0, 1),
        "theta_unit": "degree",
        'energy': np.arange(130.0,151.0, 1),
        'number': [10000],
        'particle': 'proton'
    }
    meta = Meta(
        {
            "macros": values_macros,
        }
    )

    input_data = input_generator_satellite(meta, INPUT_TEMPLATE)
    command = "../../build/satellite/geant4-satellite.exe"
    readers = [ProtoReader("deposit.proto.bin", proto_convertor=convert_satellite_proto)]
    multirun_command(input_data, command, post_processor=get_convertor(readers, "./result_130_150.hdf5", clear=True))
    return 0


if __name__ == '__main__':
    main()
