import logging

from phd.utils.run_tools import InputData, multirun_command

INPUT_TEMPLATE = """/df/project test
/df/gdml ../gdml/empty.gdml
/thunderstorm/physics standard
/thunderstorm/stacking one_generation

/gps/particle e-
/gps/direction 0 0 -1
/gps/ene/mono 100 MeV
/run/beamOn 5
"""

def input_generator():
    data = InputData(
        text=INPUT_TEMPLATE,
        path="./sim0001"
    )
    yield data

def main():
    logging.root.setLevel(logging.DEBUG)
    command = "../build/thunderstorm/geant4-thunderstorm.exe"
    multirun_command(input_generator(), command)
    return 0

if __name__ == '__main__':
    main()