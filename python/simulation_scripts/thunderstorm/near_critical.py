import logging
import os

import numpy as np
from dataforge import Meta
from phd.thunderstorm.convert_to_hdf5 import CylinderProtoSet
from phd.utils.hdf5_tools import get_convertor, ProtoSetReader
from phd.utils.run_tools import multirun_command, general_input_generator

ROOT_PATH = os.path.dirname(__file__)

INPUT_TEMPLATE = """/npm/geometry/type gdml
/npm/geometry/gdml ${path}
/npm/thunderstorm/physics ${physics}
/npm/thunderstorm/minimal_energy ${cut} MeV
/npm/thunderstorm/stacking/electron false
/npm/thunderstorm/stacking/positron false
/npm/thunderstorm/stacking/gamma false
/npm/thunderstorm/stacking/save_gamma true
/npm/thunderstorm/stacking/save_electron true

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

    gdml_template = os.path.join(ROOT_PATH, "template", "cylinder.gdml")

    values_gdml = {
    'height' : [0],
    'cellHeight' : [600],
    'fieldValueZ' : [10e-4], #np.arange(2,11)*1e-4,
    }

    values_macros = {
    "physics" : ["standard_opt_4"],
        "cut" : [0.005],
    'number' : [int(10)],
    'energy' : np.arange(),
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
    command = "../../build/thunderstorm/geant4-thunderstorm.exe"
    readers = [ProtoSetReader("stacking_simple.bin", CylinderProtoSet)]
    multirun_command(input_data, command, post_processor=get_convertor(readers, "./result.hdf5", clear=False))
    return 0

if __name__ == '__main__':
    main()