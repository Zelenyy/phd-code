import logging
import os
from string import Template

import pyinotify
from phd.thunderstorm.critical_energy import create_gdml, CriticalEnergyProcessor, G4CinServerHandler, INPUT_TEMPLATE
from phd.utils.run_tools import G4CinServer, CinServerParameters

ROOT_PATH = os.path.dirname(__file__)

def main():
    logging.basicConfig(filename="run.log")
    logging.root.setLevel(logging.DEBUG)

    gdml_template = os.path.join(ROOT_PATH, "template", "critical_energy.gdml")

    meta = {
        'number': 10,
        'energy': 0.05,
        "physics": "standard_opt_4",
        'height': 0,
        'field': 10e-4,
    }
    create_gdml(gdml_template, meta)
    processor = CriticalEnergyProcessor(meta)

    parameters = CinServerParameters(
        command="../build/thunderstorm/geant4-thunderstorm.exe"
    )

    with  G4CinServer(parameters) as server:
        server.start(Template(INPUT_TEMPLATE).substitute(meta))
        wm = pyinotify.WatchManager()
        notifier = pyinotify.Notifier(wm)
        wm.add_watch('.', pyinotify.ALL_EVENTS,
                     proc_fun=G4CinServerHandler(
                         server=server,
                         processor=processor
                     ))
        notifier.loop()
    return 0


if __name__ == '__main__':
    main()