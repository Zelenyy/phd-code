from string import Template
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