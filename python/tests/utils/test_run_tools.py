import logging
import os

from dataforge import Meta
from phd.thunderstorm.convert_to_hdf5 import READERS_TXT, READERS_CYLINDER_DATA
from phd.utils.hdf5_tools import get_convertor
from phd.utils.run_tools import multirun_command, general_input_generator

ROOT_PATH = os.path.dirname(__file__)

INPUT_TEMPLATE = """/df/project test
/df/gdml ${path}
/thunderstorm/physics standard
/thunderstorm/stacking one_generation

/gps/particle ${particle}
/gps/number 1
/gps/direction ${direction}
/gps/ene/mono ${energy} MeV
/gps/position 0. 0. ${posZ} m
/run/beamOn ${number}
"""


def test_run():
    logging.basicConfig(filename="run.log")
    logging.root.setLevel(logging.DEBUG)
    gdml_template = os.path.join(ROOT_PATH,"..", "..","simulation_scripts","thunderstorm", "template", "dwyer2003.gdml")
    print(gdml_template)
    values_gdml = {
        'height': [5000, 6000],
        'fieldValueZ': [2e-4, 1e-4],
    }
    values_macros = {
        'number': [int(1e2)],
        'energy': [i for i in range(1, 3)],
        'posZ': [49.5],
        'direction': ['0 0 -1', '0.5 0 -0.5'],
        'particle': 'e-'
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
    multirun_command(input_data, command, post_processor=get_convertor(readers, "./result.hdf5"))
    return 0

if __name__ == '__main__':
    test_run()