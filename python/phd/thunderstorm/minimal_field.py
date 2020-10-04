import star
import numpy as np
from phd.thunderstorm import atmosphere

def get_minimal_field(height = 0.0):
    """

    :param height: meters
    :return:
    """
    material = star.electron.PredefinedMaterials.AIR_DRY_NEAR_SEA_LEVEL
    density = atmosphere.ISACalculator.density(height)  # kg/m3
    data = star.electron.calculate_estar_table(material)
    stopPower = data["stopping_power_total"].min()
    energy_index = data["stopping_power_total"].argmin()
    energy = data["energy"][energy_index]
    return stopPower*density


