
# Simple atmosphere composition
import math
import star
import numpy as np

air_simple = {'N2': 80, 'O2': 20}

# Accuracy atmosphere composition
air_accuracy = {
    'N2' : 75.50733,
    'O2' : 23.15,
    'Ar' : 1.292,
    'CO2' : 0.046,
    'Ne' : 0.0014,
    'Kr' : 0.003,
    'CH4' : 0.000084,
    'He' : 0.000073,
    'H2' : 0.00008,
    'Xe' : 0.00004
}

T0 = 288.15 # kelvin
TEMPERATURE_GRAD = -0.0065 # kelvin / m
P0 = 101325 # pascal;
g = 9.80665 # m / (s * s)
M = 0.0289644 # kg / mole
R = 8.31447 # joule / (kelvin * mole)


class ISACalculator:
    @staticmethod
    def density(height: np.ndarray) -> np.ndarray:
        """
        ISA density in kg/m3m
        
        """
        temperature = ISACalculator.temperature(height)
        pressure = ISACalculator.pressure(height)
        return pressure*M/(R*temperature)

    @staticmethod
    def temperature(height: np.ndarray) -> np.ndarray:
        return T0 + TEMPERATURE_GRAD*height

    @staticmethod
    def pressure(height: np.ndarray) -> np.ndarray:
        temperature = ISACalculator.temperature(height)
        return  P0*np.exp(-1*M*g*height/(R*temperature))

def custom_star_air(height):
    material = star.electron.PredefinedMaterials.AIR_DRY_NEAR_SEA_LEVEL
    material = star.electron.load_material(material)
    material.density = ISACalculator.density(height)
    return material