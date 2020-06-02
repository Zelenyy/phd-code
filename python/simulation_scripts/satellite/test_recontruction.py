import tables
from phd.satellite.geant4_server import DetectorMode, Geant4Server
import numpy as np
from phd.satellite.run import request_generator
from phd.satellite.satellite_pb2 import Run
from phd.satellite.single_processing import load_likelihood_factory, SingleProcessing

reconstructed_dtype = np.dtype(
[
    ("original", "d", (3,)),
    ("reconstructed", "d", (3,)),
    ("success", np.bool ),
]
)


def add_row(table, value, result):
    row = table.row
    row["original"] = np.array([value["energy"], value["theta"], value["shift"]])
    row["reconstructed"] = result.x
    row["success"] = result.success
    row.append()
    return 0

def main():
    path = "/mnt/storage2/phd/data/satellite/polistyrene/mean_mesh.hdf5"
    lh_fact = load_likelihood_factory(path, particle="proton")
    single_processing = SingleProcessing(lh_fact)
    meta = {
        "gdml": "../satellite.gdml",
        "mode": DetectorMode.SINGLE.value,
        "port": 8777
    }

    values_macros = {
        "radius": 0.15,
        "shift": [0.0, 0.005, 0.016],
        "theta": [0.0, 10.0, 20., 30.0, 50.0, 70.0],
        'energy': np.arange(10.0, 150.1, 1.0),
        'number': [10],
        'particle': 'proton'
    }

    with Geant4Server(["../build/satellite/geant4-satellite.exe server"], meta) as server:
        run = Run()
        with tables.open_file("reconstruction.hdf5", "w") as fout:
            table = fout.create_table(fout.root, "proton_1", description=reconstructed_dtype)
            for text, value in request_generator(values_macros, [0.0, 0.0, 0.1]):
                data = server.send(text)
                run.ParseFromString(data)
                for event in run.event:
                    result = single_processing.process(np.array(event.deposit))
                    add_row(table, value, result)
                table.flush()
                    # print(value["energy"], lh_fact.energy_normilizer.unnormalize(result.x[0]))

if __name__ == '__main__':
    main()