import logging
import os
from string import Template

from dataforge import Meta
from phd.thunderstorm.convert_to_hdf5 import READERS_TXT, READERS_CYLINDER_DATA
from phd.utils.hdf5_tools import get_convertor
from phd.utils.run_tools import InputData, multirun_command, dir_name_generator, values_from_dict, create_gdml

ROOT_PATH = os.path.dirname(__file__)

INPUT_TEMPLATE = """/df/project test
/df/gdml ${path}
/thunderstorm/physics ${physics}
/thunderstorm/stacking one_generation

/gps/particle ${particle}
/gps/number 1
/gps/direction ${direction}
/gps/ene/mono ${energy} MeV
/gps/position 0. 0. ${posZ} m
/run/beamOn ${number}
"""

def input_generator():
    gdml_template = os.path.join(ROOT_PATH, "template", "diff_models_0.gdml")
    values_gdml = {
    'height' : [0],
    'cellHeight' : [600],
    'fieldValueZ' : [0, 3e-4, 10e-4],
    }
    paths = list(map(lambda x: os.path.join("..", x),
                     create_gdml(gdml_template, values_gdml)))
    values_gps = {
    "physics" : ["standard","standard_opt_1","standard_opt_2","standard_opt_3",  "standard_opt_4", "penelopa", "livermore", "emlowepphysics"],
    "path" : paths,
    'number' : [int(5)],
    'energy' : [10],
    'posZ' : [399.9],
    'direction' : ['0 0 -1'],
    'particle' : 'e-'
                  }

    template = Template(INPUT_TEMPLATE)
    for path, values in zip(
            dir_name_generator(".", "sim"),
            values_from_dict(values_gps)
    ):
        text = template.substitute(values)
        data = InputData(
            text=text,
            path=path,
            values=Meta(values)
        )
        yield data


def main():
    logging.basicConfig(filename = "run.log")
    logging.root.setLevel(logging.DEBUG)

    command = "../build/thunderstorm/geant4-thunderstorm.exe"
    readers = READERS_CYLINDER_DATA + READERS_TXT
    multirun_command(input_generator(), command, post_processor=get_convertor(readers, "./result.hdf5", True))
    return 0

if __name__ == '__main__':
    main()