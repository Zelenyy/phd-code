from dataclasses import dataclass
from string import Template
from typing import Union

import numpy as np
from dataforge import Meta
from phd.utils.run_tools import dir_name_generator, values_from_dict, InputData

def input_generator_satellite(meta: Meta, macros_template: str, init_pos):
    macros_template = Template(macros_template)
    for path, values in zip(
            dir_name_generator(".", "sim"),
            values_from_dict(meta["macros"])
    ):
        theta = values["theta"]
        radius = values["radius"]
        theta = np.deg2rad(theta)

        shift = values["shift"]

        posX = radius*np.sin(theta)
        posZ = radius*np.cos(theta)
        dirX = -np.sin(theta)
        dirZ = -np.cos(theta)
        values["posX"] = posX + init_pos[0] + shift
        values["posZ"] = posZ + init_pos[2]
        values["dirX"] = dirX
        values["dirZ"] = dirZ
        text = macros_template.substitute(values)
        input_data_meta = {
            "macros": values,
        }
        data = InputData(
            text=text,
            path=path,
            values=Meta(input_data_meta)
        )
        yield data


@dataclass
class QueueData:
    meta : dict
    data : Union[str, bytes]

from tqdm import tqdm

def request_generator(values, init_pos):
    """
    """

    text = """/gps/particle ${particle}
/gps/number 1
/gps/direction ${dirX} 0.0 ${dirZ}
/gps/ene/mono ${energy} MeV
/gps/position ${posX} 0. ${posZ} m
/run/beamOn ${number}
"""
    template = Template(text)
    for value in values_from_dict(values):
        theta = value["theta"]
        radius = value["radius"]
        theta = np.deg2rad(theta)

        shift = value["shift"]

        posX = radius*np.sin(theta)
        posZ = radius*np.cos(theta)
        dirX = -np.sin(theta)
        dirZ = -np.cos(theta)
        value["posX"] = posX + init_pos[0] + shift
        value["posZ"] = posZ + init_pos[2]
        value["dirX"] = dirX
        value["dirZ"] = dirZ

        request  = template.substitute(value)
        yield QueueData(value, request)