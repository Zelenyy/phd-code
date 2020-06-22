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

    max_theta_plus = 65.0
    max_theta_minus = 75.0

    values_macros = {
        "radius": 0.15,
        "shift": np.arange(0.0, 0.011, 0.002),
        "theta": np.arange(-max_theta_minus, max_theta_plus+1, 2),
        'energy': np.arange(10.0, 120.1, 1.0),
        'number': [10000],
        'particle': 'proton'
        # 'particle': 'e-'
    }

    server_run(generate_meta(), values_macros, MessageParameters("proton.hdf5", "mean"), n_workers=12)
    return 0

if __name__ == '__main__':
    main()
