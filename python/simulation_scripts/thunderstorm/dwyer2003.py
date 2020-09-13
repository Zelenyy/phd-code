import logging
import os

from dataforge import Meta
from phd.thunderstorm.convert_to_hdf5 import CylinderProtoSet, HistogramProtoSet
from phd.thunderstorm.for_runs import input_generator_custom_gdml_dwyer2003, GGFieldHeigth
from phd.utils.hdf5_tools import get_convertor, ProtoSetReader
from phd.utils.run_tools import multirun_command

ROOT_PATH = os.path.dirname(__file__)

INPUT_TEMPLATE = """/df/project thunderstorm
/df/gdml ${path}
/thunderstorm/physics standard_opt_4
/thunderstorm/stacking dwyer2003
/thunderstorm/tracking tree
/thunderstorm/cut/energy ${cut}

/gps/particle ${particle}
/gps/number 1
/gps/direction ${direction}
/gps/ene/mono ${energy} MeV
/gps/position 0. 0. ${posZ} m
/run/beamOn ${number}
"""

def main():
    logging.basicConfig(filename="run.log")
    logging.root.setLevel(logging.DEBUG)

    gdml_template = os.path.join(ROOT_PATH, "template", "cylinder.gdml")

    values_macros = {
        "cut": [0.05],
        "physics": ["standard_opt_4"],
        'number': [2],
        'energy': [1.0],
        'direction': ['0 0 -1'],
        'particle': 'e-'
    }
    meta = Meta(
        {
            "macros": values_macros,
        }
    )
    # fields = [10e-4, 7e-4, 6.0e-4, 5.5e-4, 5.0e-4, 4.5e-4, 4.0e-4]
    # heights = [200, 300, 400, 400, 500, 700, 1000]
    fields = [10e-4, 7e-4]
    heights = [200, 300]
    ggfh = GGFieldHeigth(fields, heights)
    input_data = input_generator_custom_gdml_dwyer2003(meta, gdml_template, INPUT_TEMPLATE, ggfh)
    command = "../../build/thunderstorm/geant4-thunderstorm.exe"
    readers = [
        ProtoSetReader("treeTracking.bin", CylinderProtoSet),
        ProtoSetReader("gammaSeed.bin", CylinderProtoSet),
        ProtoSetReader("positronSeed.bin", CylinderProtoSet),
        ProtoSetReader("histogram.bin", HistogramProtoSet)]
    multirun_command(input_data, command, post_processor=get_convertor(readers, "./result.hdf5", clear=True))
    return 0


if __name__ == '__main__':
    main()
