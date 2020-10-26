import logging
import os
from string import Template

from dataforge import Meta
from phd.thunderstorm import atmosphere
from phd.thunderstorm.convert_to_hdf5 import CylinderProtoSet
from phd.thunderstorm.critical_energy import get_critical_energy
from phd.utils.hdf5_tools import get_convertor, ProtoSetReader
from phd.utils.run_tools import multirun_command, InputData, create_gdml, dir_name_generator, values_from_dict

ROOT_PATH = os.path.dirname(__file__)

INPUT_TEMPLATE = """/npm/geometry/type gdml
/npm/geometry/gdml ${path}
/npm/thunderstorm/physics ${physics}
/npm/thunderstorm/minimal_energy ${min_energy} MeV
/npm/thunderstorm/stacking/electron false
/npm/thunderstorm/stacking/positron false
/npm/thunderstorm/stacking/gamma false
/npm/thunderstorm/stacking/save_gamma false
/npm/thunderstorm/stacking/save_electron true
/npm/thunderstorm/stacking/save_electron_cut ${min_energy} MeV

/gps/particle e-
/gps/number 1
/gps/direction 0 0 -1
/gps/ene/mono ${energy} MeV
/gps/position 0.0 0.0 499.0 m
/run/beamOn ${number}
exit
"""

import numpy as np

def input_generator_critical_energy():
    gdml_template = os.path.join(ROOT_PATH, "template", "critical_energy.gdml")
    macros_template = Template(INPUT_TEMPLATE)
    count = 0
    zero_field_min, zero_field_max = 3.0, 11.0
    zero_density = atmosphere.ISACalculator.density(0.0)
    gdml_index = 0
    for height in np.arange(0.0, 16000.0, 1000.0):
        density = atmosphere.ISACalculator.density(height)
        ratio = zero_density/density
        field_min = np.floor(zero_field_min/ratio)
        field_max = np.ceil(zero_field_max/ratio)
        for field in np.linspace(field_min, field_max, 10):
            temp = get_critical_energy(height, field)
            if temp is None or not temp.converged:
                continue
            critical_energy = temp.root

            values_gdml = {
                'height': height,
                'field': field*1e-4,
            }

            paths, _ = create_gdml(gdml_template, values_gdml,gdml_index)
            gdml_index+=1
            gdml_path = paths[0]
            values = {
                "path": [os.path.join("..",gdml_path)],
                "physics": ["standard_opt_4"],
                'number': [1000],
                'energy': np.arange(critical_energy/2, critical_energy*2, 0.003),
                'min_energy': [critical_energy/2],
            }

            for path, values in zip(
                    dir_name_generator(".", "sim", start=count),
                    values_from_dict(values)
            ):
                count += 1
                text = macros_template.substitute(values)
                input_data_meta = {
                    "macros": values,
                    "gdml": values_gdml
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

    input_data = input_generator_critical_energy()
    command = "../../build/thunderstorm/geant4-thunderstorm.exe"
    readers = [
        ProtoSetReader("stacking_simple.bin", CylinderProtoSet)
    ]
    multirun_command(input_data, command, post_processor=get_convertor(readers, "./result.hdf5", clear=True))
    return 0


if __name__ == '__main__':
    main()
