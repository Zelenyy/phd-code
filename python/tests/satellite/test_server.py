from unittest import TestCase
import numpy as np
from phd.satellite.geant4_server import Geant4Server, DetectorMode, StartParameters, start_server_in_thread
from phd.satellite.run import request_generator
from phd.satellite.satellite_pb2 import Run
from multiprocessing import Pool

class TestServer(TestCase):
    def test_single_thread(self):
        meta = {
            "gdml": "../satellite.gdml",
            "mode": DetectorMode.SINGLE.value,
            "port": 8777
        }

        values_macros = {
            "radius": 0.15,
            "shift": [0.0],
            "theta": [0.0],
            'energy': np.arange(1.0, 3.1, 1.0),
            'number': [10],
            'particle': 'e-'
        }

        with Geant4Server(["../build/satellite/geant4-satellite.exe server"], meta) as server:
            run = Run()
            for text, value in request_generator(values_macros, [0.0, 0.0, 0.1]):
                data = server.send(text)
                run.ParseFromString(data)
                for event in run.event:
                    print(event.deposit[0])

    def test_multy_thread(self):
        meta = {
            "gdml": "../satellite.gdml",
            "mode": DetectorMode.SINGLE.value,
            "port": 8777
        }

        values_macros = {
            "radius": 0.15,
            "shift": [0.0],
            "theta": [0.0],
            'energy': np.arange(1.0, 3.1, 1.0),
            'number': [10],
            'particle': 'e-'
        }

        parameters = []

        port = 8777
        import copy
        for i in range(4):
            meta["port"] = port + i
            parameters.append(
                StartParameters(
                    copy.deepcopy(meta),
                    request_generator(values_macros, [0.0, 0.0, 0.1]),
                    Run
                )
            )
        with Pool(4) as p:
            for result in p.imap_unordered(start_server_in_thread, parameters):
                print(result)