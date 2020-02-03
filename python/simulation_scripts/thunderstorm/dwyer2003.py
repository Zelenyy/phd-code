import logging
import os

from dataforge import Meta
from phd.thunderstorm.convert_to_hdf5 import READERS_TXT, READERS_CYLINDER_DATA
from phd.utils.hdf5_tools import get_convertor
from phd.utils.run_tools import multirun_command, \
    create_one_file, input_generator_custom_gdml

ROOT_PATH = os.path.dirname(__file__)

INPUT_TEMPLATE = """/df/project test
/df/gdml ${path}
/thunderstorm/physics ${physics}
/thunderstorm/stacking ${stacking}
/thunderstorm/cut/energy ${cut}

/gps/particle ${particle}
/gps/number 1
/gps/direction ${direction}
/gps/ene/mono ${energy} MeV
/gps/position 0. 0. ${posZ} m
/run/beamOn ${number}
"""


def gdml_generator(template_file):
    os.makedirs("./gdml", exist_ok=True)
    fields = [8e-4, 6e-4, 5.5e-4, 5.2e-4]
    heights = [100, 200, 300, 400]
    paths = []
    values_gdml = []

    with open(template_file) as fin:
        gdml_template = fin.read()
    for indx, pair in enumerate(zip(heights, fields)):
        height, field = pair
        temp_gdml = {
            'height': 0,
            'cellHeight': height,
            'fieldValueZ': field,
        }
        path = os.path.join("./gdml", "{}.gdml".format(indx))
        paths.append(path)
        values_gdml.append(temp_gdml)
        create_one_file(gdml_template, path, temp_gdml)
    return paths, values_gdml


def main():
    logging.basicConfig(filename="run.log")
    logging.root.setLevel(logging.DEBUG)

    gdml_template = os.path.join(ROOT_PATH, "template", "diff_models_0.gdml")

    values_macros = {
        "physics": ["standard_opt_4"],
        "stacking": "dwyer2003",
        "cut": [0.05],
        'number': [int(10)],
        'energy': [1.0],
        'posZ': [200],
        'direction': ['0 0 -1'],
        'particle': 'e-'
    }
    meta = Meta(
        {
            "macros": values_macros,
        }
    )

    input_data = input_generator_custom_gdml(meta, gdml_template, INPUT_TEMPLATE, gdml_generator)
    command = "../build/thunderstorm/geant4-thunderstorm.exe"
    readers = READERS_CYLINDER_DATA + READERS_TXT
    multirun_command(input_data, command, post_processor=get_convertor(readers, "./result.hdf5", clear=True))
    return 0


if __name__ == '__main__':
    main()
