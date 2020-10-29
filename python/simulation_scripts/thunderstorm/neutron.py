import argparse
import logging
from string import Template

from dataforge import Meta
from phd.thunderstorm.convert_to_hdf5 import CylinderProtoSet
from phd.utils.hdf5_tools import get_convertor, ProtoSetReader
from phd.utils.run_tools import multirun_command, dir_name_generator, values_from_dict, InputData

DEFOULT_PATH = "../NeutronFullAtmosphere.gdml"
SPACECART_PATH = "../SpaceCraftFullAtmosphere.gdml"


INPUT_TEMPLATE="""/npm/geometry/type gdml
/npm/geometry/gdml ${path}
/npm/thunderstorm/physics withoutEmStandard
/npm/thunderstorm/minimal_energy 10.0 MeV
/npm/thunderstorm/stacking/electron false
/npm/thunderstorm/stacking/positron false
/npm/thunderstorm/stacking/gamma true
/npm/thunderstorm/stacking/save_gamma false
/npm/thunderstorm/stacking/save_electron false
/npm/thunderstorm/stacking/save_neutron true
/npm/thunderstorm/tracking/save_gamma true
/npm/thunderstorm/tracking/save_electron false

/gps/particle gamma
/gps/position 0.0 0.0 0.0 meter
/gps/direction 0 0 1
/gps/ene/mono ${energy} MeV
/run/beamOn ${number}
exit
"""

def input_generator_neutron(gdml_path, physics):
    values = {
        "path": [gdml_path],
        "physics": [physics],
        'number': [10000 for i in range(10)],
        'energy': [100],
    }
    macros_template = Template(INPUT_TEMPLATE)
    for path, values in zip(
            dir_name_generator(".", "sim"),
            values_from_dict(values)
    ):
        text = macros_template.substitute(values)
        input_data_meta = {
            "macros": values,
            "gdml": {}
        }
        data = InputData(
            text=text,
            path=path,
            values=Meta(input_data_meta)
        )
        yield data

def create_parser():
    parser = argparse.ArgumentParser(description='Run neutron simulation')
    parser.add_argument('--without-standard', action='store_true',
                        help='Simulate without stanard gamma intarection')
    parser.add_argument('--with-spacecraft', action='store_true',
                        help='Add spacecraft')
    parser.add_argument("--output", "-o", action="store", help="Output file name", default="result.hdf5")
    return parser

def main():
    args = create_parser().parse_args()
    logging.basicConfig(filename="run.log")
    logging.root.setLevel(logging.DEBUG)

    gdml_path = DEFOULT_PATH
    physics = "FTFP_BERT_opt4"
    if args.with_spacecraft:
        gdml_path = SPACECART_PATH
    if args.without_standard:
        physics = "withoutEmStandard"
    input_data = input_generator_neutron(gdml_path, physics)
    command = "../../build/thunderstorm/geant4-thunderstorm.exe"
    readers = [
        ProtoSetReader("stacking_simple.bin", CylinderProtoSet),
        ProtoSetReader("tracking_post.bin", CylinderProtoSet)
    ]
    multirun_command(input_data, command, post_processor=get_convertor(readers, args.output, clear=True), n_cpu_cores=12)
    return 0


if __name__ == '__main__':
    main()