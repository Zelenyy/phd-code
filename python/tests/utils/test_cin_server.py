import os
import threading
import unittest
import pyinotify
from phd.utils.run_tools import CinServerParameters, G4CinServer

START_TEXT="""/npm/geometry/type gdml
/npm/geometry/gdml /home/zelenyy/npm/phd/phd-code/cxx/empty.gdml
/npm/thunderstorm/stacking/save_electron true
separator
"""

MESSEGE = """/gps/particle gamma
/gps/position 0.0 0.0 0.0 meter
/gps/direction 0 0 -1
/gps/ene/mono 0.05 MeV
/run/beamOn 10
separator
"""

class SimpleStackingHandler(pyinotify.ProcessEvent):
    # mask = pyinotify.IN_CLOSE_WRITE | pyinotify.IN_CREATE

    def my_init(self, server: G4CinServer):
        self.server = server
        self.count = 0
        self.server.send(MESSEGE)

    def process_IN_CREATE(self, event):
        print("Create", event)

    def process_IN_CLOSE_WRITE(self, event):
        print("close_write", event)
        if event.name == "stacking_simple.bin":
            print(os.path.getsize(event.pathname))
            os.remove(event.pathname)
            self.count+=1
            if self.count > 5:
                raise KeyboardInterrupt
            self.server.send(MESSEGE)


class CinServerTest(unittest.TestCase):

    def test_server(self):
        parameters = CinServerParameters(
            command="/home/zelenyy/npm/phd/phd-code/cxx/cmake-build-debug/thunderstorm/geant4-thunderstorm.exe"
        )
        with G4CinServer(parameters) as server:
            server.start(START_TEXT)
            server.send(MESSEGE)
            server.send(MESSEGE)

    def test_inotify(self):
        parameters = CinServerParameters(
            command="/home/zelenyy/npm/phd/phd-code/cxx/cmake-build-debug/thunderstorm/geant4-thunderstorm.exe"
        )
        if os.path.exists("stacking_simple.bin"):
            os.remove("stacking_simple.bin")
        with G4CinServer(parameters) as server:
            server.start(START_TEXT)
            wm = pyinotify.WatchManager()
            notifier = pyinotify.Notifier(wm)
            wm.add_watch('.', pyinotify.ALL_EVENTS, proc_fun=SimpleStackingHandler(server=server))
            notifier.loop()


if __name__ == '__main__':
    unittest.main()