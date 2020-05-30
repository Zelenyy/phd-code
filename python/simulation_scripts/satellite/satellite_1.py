import logging
import os

import numpy as np
from dataforge import Meta
from phd.satellite.geant4_server import Geant4Server, DetectorMode
from phd.satellite.mean_table import MeanTable
from phd.satellite.run import input_generator_satellite, request_generator
from phd.satellite.satellite_pb2 import MeanRun
from phd.utils.run_tools import multirun_command



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

    meta = {
        "gdml" : "../satellite.gdml",
        "mode" : DetectorMode.SUM.value
    }

    with Geant4Server(["../build/satellite/geant4-satellite.exe server"], meta) as server, MeanTable("electron.hdf5") as mean_table:
        run = MeanRun()

        values_macros = {
            "radius": 0.15,
            "shift": np.arange(0.0, 0.016, 0.001),
            "theta": np.arange(0.0, 91.0, 1),
            'energy': np.arange(1.0, 15.1, 0.5),
            'number': [1000],
            'particle': 'e-'
        }

        for text, value in request_generator(values_macros, [0.0, 0.0, 0.1]):
            data = server.send(text)
            run.ParseFromString(data)
            mean_table.append_from_mean_run(run, value)
    return 0

if __name__ == '__main__':
    main()
