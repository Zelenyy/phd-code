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

INPUT_TEMPLATE = """/npm/geometry/type gdml
/npm/geometry/gdml ../../build/satellite/gdml/satellite.gdml
/npm/visualization false
/npm/satellite/detector ${mode}
/npm/satellite/output file

/gps/particle ${particle}
/gps/number 1
/gps/direction ${dirX} 0.0 ${dirZ}
/gps/ene/mono ${energy} MeV
/gps/position ${posX} 0. ${posZ} m
/run/beamOn ${number}
exit
"""





def main():
    logging.basicConfig(filename="run.log")
    logging.root.setLevel(logging.DEBUG)
    OUTPUT_FILE = "result.hdf5"
    values_macros = {
        "mode" : "single", # or "sum" Сожранение или всех симуляций или среднего значения.
        "radius" : 0.15,
        # "shift": [0.0, 0.005, 0.016],
        # "theta": [0.0, 10.0, 20., 30.0, 50.0, 70.0],
        "shift" : 0.0, # Сдвиг относительно центральной оси, может быть списком
        "theta" : 0.0, # Отклонение от центральной оси, может быть списком
        "theta_unit": "degree",
        'energy': np.arange(10.0,15.1, 1.0), # Набор энергий для симуляции
        'number': [100], # Число событий в одной симуляции
        # 'particle': 'e-'
        'particle': 'proton' # Запускаемая частица
    }
    meta = Meta(
        {
            "macros": values_macros,
        }
    )

    input_data = input_generator_satellite(meta, INPUT_TEMPLATE, init_pos=[0.0,0.0, 0.1])
    command = "../../build/satellite/geant4-satellite.exe"
    readers = [ProtoReader("deposit.proto.bin", proto_convertor=convert_satellite_proto)]
    multirun_command(input_data, command, post_processor=get_convertor(readers, OUTPUT_FILE, clear=True))
    return 0


if __name__ == '__main__':
    main()
