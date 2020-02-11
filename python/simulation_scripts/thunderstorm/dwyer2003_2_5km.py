import logging
import os
from string import Template

from dataforge import Meta
from phd.thunderstorm.convert_to_hdf5 import READERS_TXT, READERS_CYLINDER_DATA, READERS_CYLINDER_ID_DATA, \
    READER_TREE_SOCKET_DATA
from phd.utils.hdf5_tools import get_convertor
from phd.utils.run_tools import multirun_command, \
    create_one_file, dir_name_generator, values_from_dict, InputData

ROOT_PATH = os.path.dirname(__file__)

INPUT_TEMPLATE = """/df/project test
/df/gdml ${path}
/thunderstorm/physics standard_opt_4
/thunderstorm/stacking dwyer2003
/thunderstorm/tracking tree_socket
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
    fields = [5e-4, 3.5e-4, 3.0e-4]
    heights = [200, 400, 600]
    paths = []
    values_gdml = []

    with open(template_file) as fin:
        gdml_template = fin.read()
    for indx, pair in enumerate(zip(heights, fields)):
        height, field = pair
        temp_gdml = {
            'height': 5000,
            'cellHeight': height,
            'fieldValueZ': field,
        }
        path = os.path.join("./gdml", "{}.gdml".format(indx))
        paths.append(path)
        values_gdml.append(temp_gdml)
        create_one_file(gdml_template, path, temp_gdml)
    return paths, values_gdml


def input_generator_custom_gdml_dwyer2003(meta: Meta, gdml_template_file: str, macros_template: str, gdml_generator):
    paths, values_gdml = gdml_generator(gdml_template_file)
    paths = list(map(lambda x: os.path.join("..", x), paths))
    meta["macros"]["path"] = paths
    macros_template = Template(macros_template)
    for path, values in zip(
            dir_name_generator(".", "sim"),
            values_from_dict(meta["macros"])
    ):
        path_gdml = values["path"]
        indx = paths.index(path_gdml)
        gdml = values_gdml[indx]
        values["posZ"] = gdml["cellHeight"]/2 - 0.1
        text = macros_template.substitute(values)
        input_data_meta = {
            "macros": values,
            "gdml": gdml
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

    gdml_template = os.path.join(ROOT_PATH, "template", "diff_models_0.gdml")

    values_macros = {
        "cut": [0.05],
        'number': [10],
        'energy': [1.0],
        'direction': ['0 0 -1'],
        'particle': 'e-'
    }
    meta = Meta(
        {
            "macros": values_macros,
        }
    )

    input_data = input_generator_custom_gdml_dwyer2003(meta, gdml_template, INPUT_TEMPLATE, gdml_generator)
    command = "../../build/thunderstorm/geant4-thunderstorm.exe"
    readers = READERS_CYLINDER_ID_DATA + READERS_TXT + READER_TREE_SOCKET_DATA
    multirun_command(input_data, command, post_processor=get_convertor(readers, "./result.hdf5", clear=True))
    return 0


if __name__ == '__main__':
    main()
