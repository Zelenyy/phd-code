import logging
import os
from string import Template

import numpy as np
import tables
from dataforge import Meta
from phd.thunderstorm.convert_to_hdf5 import READERS_TXT, READERS_CYLINDER_DATA, get_named_number_readers, \
    get_named_cylinder_readers
from phd.utils.convertor_tools import theta_to_direction
from phd.utils.hdf5_tools import get_convertor
from phd.utils.run_tools import multirun_command, general_input_generator, InputData, run_command

ROOT_PATH = os.path.dirname(__file__)

INPUT_TEMPLATE = """/df/project test
/df/gdml ../gdml/reversed_electron.gdml
/thunderstorm/physics standard_opt_4
/thunderstorm/stacking particle_cylinder
/thunderstorm/addParticleInPCS gamma
/thunderstorm/addParticleInPD e-
/thunderstorm/cut/energy ${cut}

/gps/particle e-
/gps/number 1
/gps/direction ${direction}
/gps/ene/mono ${energy} MeV
/gps/position 0. 0. 0. m
/run/beamOn ${number}
"""


def get_readers():
    readers = []
    particles, txt = get_named_number_readers("./run", "particle_detector")
    binReader = get_named_cylinder_readers(particles, "particle_detector")
    readers = readers + txt + binReader
    particles, txt = get_named_number_readers("./run", "particle_cylinder")
    binReader = get_named_cylinder_readers(particles, "particle_cylinder")
    readers = readers + txt + binReader
    return readers

def run_simualtion(energy_parallel, energy_perpendicular, macros_template, values):
    energy = (energy_parallel**2 + energy_perpendicular**2)**0.5
    theta = np.arctan(energy_perpendicular/energy_parallel)
    values["direction"] = theta_to_direction(theta)
    values["energy"] = energy
    text = Template(macros_template).substitute(values)

    os.makedirs("./run", exist_ok=True);
    input_data = InputData("./run", text, values)
    parameters = input_data, "../build/thunderstorm/geant4-thunderstorm.exe"
    return run_command(parameters)


def process(path, iter = 0):
    if iter == 0:
        name = "/run"
    else:
        name = "/run_re_{}".format(iter)

    with tables.open_file(path) as h5file:
        table = h5file.get_node(name, "particle_detector_number")
        data = table.read()
        indx = data['electron'] == 0
        probability = np.sum(indx)/table.attrs["values_number"]
    return probability

def reverse_electron():
    gdml_template = os.path.join(ROOT_PATH, "template", "reversed_electron.gdml")
    values = {
    'height' : 0,
    'fieldValueZ' : 8e-4,
    "cut" : 0.05,
    'number' : 100,
    }
    os.makedirs("gdml", exist_ok=True)
    with open("gdml/reversed_electron.gdml", "w") as fout, open(gdml_template) as fin:
        text = fin.read()
        text = Template(text) 
        text = text.substitute(values)
        fout.write(text)

    iter = 0
    energy_parallel, energy_perpendicular = 1.0, 0.0
    step = 0.01

    for i in np.arange(0.05, 1.01, 0.05):
        for j in np.arange(0.05, 1.01, 0.05):
            input_data = run_simualtion(i, j, INPUT_TEMPLATE, values)
            readers = get_readers()
            post_run_processor = get_convertor(readers, "./result.hdf5", clear=True)
            post_run_processor(input_data)



    # while True:
    #     print("Start simulation with {:.2}, {:.2}".format(energy_parallel, energy_perpendicular))
    #     input_data = run_simualtion(energy_parallel, energy_perpendicular, INPUT_TEMPLATE, values)
    #     readers = get_readers()
    #     post_run_processor = get_convertor(readers, "./result.hdf5", clear=True)
    #     post_run_processor(input_data)
    #     probability = process("./result.hdf5", iter)
    #     print(probability)
    #     iter +=1
    #     if probability>0.7:
    #         energy_perpendicular += step
    #     else:
    #         energy_parallel -= step
    #     if energy_parallel < 0 or energy_perpendicular>1:
    #         break

def main():
    logging.basicConfig(filename = "run.log")
    logging.root.setLevel(logging.DEBUG)
    readers = READERS_CYLINDER_DATA + READERS_TXT

    reverse_electron()

    return 0

if __name__ == '__main__':
    main()