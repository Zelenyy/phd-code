import logging
import os
from string import Template

import numpy as np
from dataforge import Meta
from phd.satellite.covert_to_hdf5 import convert_satellite_proto
from phd.satellite.run import input_generator_satellite
from phd.utils.hdf5_tools import get_convertor, ProtoReader
from phd.utils.run_tools import multirun_command, \
    dir_name_generator, values_from_dict, InputData

ROOT_PATH = os.path.dirname(__file__)

INPUT_TEMPLATE = """/df/project test
/df/gdml ../../satellite.gdml
/satellite/output file
/satellite/detector ${mode}

/gps/particle ${particle}
/gps/number 1
/gps/direction ${dirX} 0.0 ${dirZ}
/gps/ene/mono ${energy} MeV
/gps/position ${posX} 0. ${posZ} m
/run/beamOn ${number}
"""





def main():
    logging.basicConfig(filename="run.log")
    logging.root.setLevel(logging.DEBUG)

    values_macros = {
        "mode" : "single",
        "radius" : 0.15,
        "shift" : 0,
        "theta" : np.arange(0.0,71.0, 1),
        # "theta" : [30], #[0.0],
        "theta_unit": "degree",
        'energy': np.arange(10.0,151.0, 1),
        'number': [1000],
        'particle': 'proton'
    }
    meta = Meta(
        {
            "macros": values_macros,
        }
    )

    input_data = input_generator_satellite(meta, INPUT_TEMPLATE, init_pos=[0.0,0.0, 0.1])
    command = "../../build/satellite/geant4-satellite.exe"
    readers = [ProtoReader("deposit.proto.bin", proto_convertor=convert_satellite_proto)]
    # for data in input_data:
    #     print(data.text)
    multirun_command(input_data, command, post_processor=get_convertor(readers, "./proton.hdf5", clear=True))
    return 0


if __name__ == '__main__':
    main()
