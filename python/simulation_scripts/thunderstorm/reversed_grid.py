import logging
import os
from string import Template

import numpy as np
from dataforge import Meta
from phd.thunderstorm.convert_to_hdf5 import get_named_cylinder_readers, get_named_number_readers_1
from phd.utils.convertor_tools import theta_to_direction
from phd.utils.hdf5_tools import get_convertor
from phd.utils.run_tools import multirun_command, general_input_generator, create_gdml, dir_name_generator, \
    values_from_dict, InputData

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


def values_macros_gen(paths):
    for path in paths:
        for theta in np.arange(0, 91, 10):
            for energy in np.arange(0.1, 3.01, 0.05):
                if energy > 2 and theta > 55:
                    continue
                values_macros = {
                    "path": path,
                    "cut": 0.05,
                    'number': 1,
                    'energy': energy,
                    'direction': theta_to_direction(np.deg2rad(theta)),
                }
                yield values_macros

def reverse_grid_input_generator(gdml_template_file: str, macros_template: str):
    macros_template = Template(macros_template)
    values_gdml = {
        'height': [0],
        'fieldValueZ': 1e-4 * np.arange(5.0, 10.1, 0.5),
    }
    paths, values_gdml = create_gdml(gdml_template_file, values_gdml)
    paths = list(map(lambda x: os.path.join("..", x), paths))
    for path, values in zip(
            dir_name_generator(".", "sim"),
            values_macros_gen(paths)
    ):
        text = macros_template.substitute(values)
        path_gdml = values["path"]
        indx = paths.index(path_gdml)
        input_data_meta = {
            "macros": values,
            "gdml": values_gdml[indx]
        }
        data = InputData(
            text=text,
            path=path,
            values=Meta(input_data_meta)
        )
        yield data


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
    logging.basicConfig(filename="run.log")
    logging.root.setLevel(logging.DEBUG)
    gdml_template = os.path.join(ROOT_PATH, "template", "reversed_electron.gdml")
    input_data = reverse_grid_input_generator(gdml_template, INPUT_TEMPLATE)
    command = "../../build/thunderstorm/geant4-thunderstorm.exe"
    readers = get_readers()
    multirun_command(input_data, command, post_processor=get_convertor(readers, "./result.hdf5", clear=False))
    return 0


if __name__ == '__main__':
    main()
