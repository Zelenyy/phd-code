import logging
import os

import numpy as np
from dataforge import Meta
from phd.thunderstorm.convert_to_hdf5 import READERS_TXT, READERS_CYLINDER_DATA, get_named_number_readers, \
    get_named_cylinder_readers, get_named_number_readers_1
from phd.utils.convertor_tools import theta_to_direction
from phd.utils.hdf5_tools import get_convertor
from phd.utils.run_tools import multirun_command, general_input_generator

ROOT_PATH = os.path.dirname(__file__)

INPUT_TEMPLATE = """/df/project test
/df/gdml ${path}
/thunderstorm/physics standard_opt_4
/thunderstorm/stacking particle_cylinder
/thunderstorm/addParticleInPCS gamma
/thunderstorm/addParticleInPD e-
/thunderstorm/cut/energy ${cut}

/gps/particle e-
/gps/number 1
/gps/direction ${direction}
/gps/ene/mono ${energy} MeV
/gps/position 0. 0. 0. m
/run/beamOn ${number}
"""

def get_readers():
    readers = []
    txt = get_named_number_readers_1(["e-"], "particle_detector")
    binReader = get_named_cylinder_readers(["e-"], "particle_detector")
    readers = readers + txt + binReader
    txt = get_named_number_readers_1(["gamma"], "particle_cylinder")
    binReader = get_named_cylinder_readers(["gamma"], "particle_cylinder")
    readers = readers + txt + binReader
    return readers

def main():
    logging.basicConfig(filename = "run.log")
    logging.root.setLevel(logging.DEBUG)

    gdml_template = os.path.join(ROOT_PATH, "template", "reversed_electron.gdml")

    values_gdml = {
    'height' : [0],
    'fieldValueZ' : [8e-4],
    }


    energy = np.arange(0.1, 1.01, 0.05)
    theta = np.arange(0, 91, 10)

    values_macros = {
    "cut" : [0.05],
    'number' : [int(100)],
    'energy' : energy,
    'posZ' : [200],
    'direction' : list(map(theta_to_direction, np.deg2rad(theta))),
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
    readers = get_readers()
    multirun_command(input_data, command, post_processor=get_convertor(readers, "./result.hdf5", clear=True))
    return 0


if __name__ == '__main__':
    main()