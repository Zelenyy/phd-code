import logging
import os

import numpy as np
from phd.satellite.geant4_server import DetectorMode, server_run, MessageParameters



def generate_meta():
    port = 8777
    while True:
        meta = {
            "command" : ["../build/satellite/geant4-satellite.exe"],
            "gdml" : "../satellite.gdml",
            "mode" : DetectorMode.SUM.value,
            "port" : port
        }
        port += 1
        yield meta


ROOT_PATH = os.path.dirname(__file__)

def main():
    logging.basicConfig(filename="run.log")
    logging.root.setLevel(logging.DEBUG)

    meta = {
        "command" : ["../build/satellite/geant4-satellite.exe"],
        "gdml" : "../satellite.gdml",
        "mode" : DetectorMode.SUM.value,
        "port" : 8777
    }

    values_macros = {
        "radius": 0.15,
        "shift": np.arange(0.0, 0.015, 0.006),
        "theta": np.arange(-70.0, 55.0, 25),
        'energy': np.arange(10.0, 12.1, 2),
        'number': [10],
        'particle': 'proton'
        # 'particle': 'e-'
    }

    server_run(generate_meta(), values_macros, MessageParameters("proton.hdf5", "mean"), n_workers=12)
    return 0

if __name__ == '__main__':
    main()
