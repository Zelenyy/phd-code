import logging
import os

import numpy as np
from dataforge import Meta
from phd.thunderstorm.convert_to_hdf5 import READERS_TXT, READERS_CYLINDER_DATA
from phd.utils.hdf5_tools import get_convertor
from phd.utils.run_tools import multirun_command, general_input_generator

ROOT_PATH = os.path.dirname(__file__)

INPUT_TEMPLATE = """/df/project test
/df/gdml ${path}
/thunderstorm/physics ${physics}
/thunderstorm/stacking one_generation
/thunderstorm/cut/energy ${cut}

/gps/particle ${particle}
/gps/number 1
/gps/direction ${direction}
/gps/ene/mono ${energy} MeV
/gps/position 0. 0. ${posZ} m
/run/beamOn ${number}
"""

def main():
    logging.basicConfig(filename = "run.log")
    logging.root.setLevel(logging.DEBUG)

    gdml_template = os.path.join(ROOT_PATH, "template", "diff_models_0.gdml")

    values_gdml = {
    'height' : [0],
    'cellHeight' : [100, 200, 300, 400],
    'fieldValueZ' : [8e-4, 6e-4, 5.5e-4, 5.2e-4],
    }

    values_macros = {
    "physics" : ["standard_opt_4"],
        "cut" : [0.05],
    'number' : [int(1000)],
    'energy' : [1.0],
    'posZ' : [200],
    'direction' : ['0 0 -1'],
    'particle' : 'e-'
                  }
    meta = Meta(
        {
            "macros": values_macros,
            "gdml": values_gdml
        }
    )

    input_data = general_input_generator(meta, gdml_template, INPUT_TEMPLATE)
    command = "../build/thunderstorm/geant4-thunderstorm.exe"
    readers = READERS_CYLINDER_DATA + READERS_TXT
    multirun_command(input_data, command, post_processor=get_convertor(readers, "./result.hdf5", clear=True))
    return 0

if __name__ == '__main__':
    main()