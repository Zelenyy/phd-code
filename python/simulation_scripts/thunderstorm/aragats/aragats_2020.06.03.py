import logging
import os

import numpy as np
from dataforge import Meta
from phd.thunderstorm.convert_to_hdf5 import READERS_TXT, READERS_CYLINDER_DATA, ParticleDetectorProtoSet
from phd.utils.hdf5_tools import get_convertor, ProtoSetReader
from phd.utils.run_tools import multirun_command, general_input_generator, no_gdmL_input_generator

ROOT_PATH = os.path.dirname(__file__)

INPUT_TEMPLATE = """/npm/geometry/type custom
/npm/thunderstorm/physics ${physics}
/npm/thunderstorm/cut/energy ${cut}
/npm/thunderstorm/height 3750 m
/npm/thunderstorm/field_z ${field_z} kV/m

/gps/particle ${particle}
/gps/number 1
/gps/direction ${direction}
/gps/ene/mono ${energy} MeV
/gps/position 0. 0. ${posZ} m
/run/beamOn ${number}
exit
"""

def main():
    logging.basicConfig(filename = "run.log")
    logging.root.setLevel(logging.DEBUG)

    values_macros = {
    "physics" : ["standard"],
        "cut" : [0.05],
    'number' : [1000],
    'energy' : [1.0],
    'posZ' : [499.9],
    'direction' : ['0 0 -1'],
    'particle' : 'e-',
    'field_z' : [150, 220],
                  }
    meta = Meta(values_macros)

    input_data = no_gdmL_input_generator(meta, INPUT_TEMPLATE)
    command = "../../build/thunderstorm/geant4-thunderstorm.exe"
    readers = [ProtoSetReader("particle_detector.bin", ParticleDetectorProtoSet)]

    multirun_command(input_data, command, post_processor=get_convertor(readers, "./result.hdf5", clear=False))
    return 0

if __name__ == '__main__':
    main()