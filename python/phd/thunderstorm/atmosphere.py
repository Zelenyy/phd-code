import os
import star
import numpy as np
from phd.utils.gdml import GDML, tube, GDMLVolume, GDMLPhys, position
import xml.etree.ElementTree as ET
from lxml import etree

# Simple atmosphere composition
air_simple = {'N2': 80, 'O2': 20}

# Accuracy atmosphere composition
air_accuracy = {
    'N2': 75.50733,
    'O2': 23.15,
    'Ar': 1.292,
    'CO2': 0.046,
    'Ne': 0.0014,
    'Kr': 0.003,
    'CH4': 0.000084,
    'He': 0.000073,
    'H2': 0.00008,
    'Xe': 0.00004
}

T0 = 288.15  # kelvin
TEMPERATURE_GRAD = -0.0065  # kelvin / m
P0 = 101325  # pascal;
g = 9.80665  # m / (s * s)
M = 0.0289644  # kg / mole
R = 8.31447  # joule / (kelvin * mole)


class ISACalculator:
    @staticmethod
    def density(height: np.ndarray) -> np.ndarray:
        """
        height: meters
        ISA density in kg/m3m

        """
        temperature = ISACalculator.temperature(height)
        pressure = ISACalculator.pressure(height)
        return pressure * M / (R * temperature)

    @staticmethod
    def temperature(height: np.ndarray) -> np.ndarray:
        return T0 + TEMPERATURE_GRAD * height

    @staticmethod
    def pressure(height: np.ndarray) -> np.ndarray:
        temperature = ISACalculator.temperature(height)
        return P0 * np.exp(-1 * M * g * height / (R * temperature))


def custom_star_air(height):
    material = star.electron.PredefinedMaterials.AIR_DRY_NEAR_SEA_LEVEL
    material = star.electron.load_material(material)
    material.density = ISACalculator.density(height)
    return material


def read_usa_1976(path):
    dtype = np.dtype([
        ("altitude", "d"),  # meter
        ("temperature", "d"),
        ("pressure", "d"),
        ("density", "d"),
        ("viscosity", "d")
    ])

    return np.loadtxt(path, dtype=dtype)


def read_msise(path):
    dtype = np.dtype([
        ("altitude", "d"),  # meter
        ("low_temperature", "d"),  # Kelvin
        ("low_density", "d"),          # kg/m3
        ("low_pressure", "d"),  # Pascal

        ("low_molar_weight", "d"),
        ("mean_temperature", "d"),
        ("mean_density", "d"),
        ("mean_pressure", "d"),
        ("mean_molar_weight", "d"),
        ("high_temperature", "d"),
        ("high_density", "d"),
        ("high_pressure", "d"),
        ("high_molar_weight", "d")
    ])
    data = np.loadtxt(path, dtype=dtype)
    data["altitude"] *= 1000  # from km to meters
    return data


def create_full_earth_atmosphere(with_spacecraft=False):
    # http://www.braeunig.us/space/atmos.htm
    root_path = os.path.dirname(__file__)
    path_usa_1976 = os.path.join(root_path, "data", "USA_Atmosphere_1976.txt")
    path_msise90 = os.path.join(root_path, "data", "MSISE-90.txt")
    data = read_usa_1976(path_usa_1976)

    gdml = GDML(name="NeutronLaeyrs")
    z_max = 1e6
    world_solid = tube("worldSolid", 400000, 2 * z_max + 10000, luint="m")
    gdml.add_solid(world_solid)
    world = GDMLVolume("World", "G4_Galactic", "worldSolid")
    down = 0
    i_max = len(data) - 1
    # i_max = 3 # debug
    for i in range(2, i_max):
        up = data["altitude"][i + 1]
        z = up - down
        location = down + (z) / 2
        name = "h{}m".format(data["altitude"][i])
        solid_name = "solid_" + name
        mat_name = "mat_" + name
        gdml.add_solid(tube(solid_name, 390000, z, luint="m"))
        gdml.add_material(mat_name, data["density"][i] * 1e-3,
                          fraction=[("G4_AIR", 1.0)])
        vol_name = "vol_" + name
        volume = GDMLVolume(vol_name, mat_name, solid_name)
        gdml.add_volume(volume)
        world.add_phys(GDMLPhys(vol_name, position=position(z=location, unit="m")))
        down = up

    data = read_msise(path_msise90)
    data = data[data["altitude"] > down]
    data = data[data["altitude"] < z_max]

    i_max = len(data) - 1
    for i in range(i_max):
        up = data["altitude"][i + 1]
        z = up - down
        location = down + (z) / 2
        name = "h{}m".format(data["altitude"][i])
        solid_name = "solid_" + name
        mat_name = "mat_" + name
        vol_name = "vol_" + name
        gdml.add_solid(tube(solid_name, 390000, z, luint="m"))
        if i == i_max-1 and with_spacecraft:
            mat_name = "G4_POLYSTYRENE"
        else:
            gdml.add_material(mat_name, data["mean_density"][i] * 1e-3,
                              fraction=[("G4_AIR", 1.0)])

        volume = GDMLVolume(vol_name, mat_name, solid_name)
        gdml.add_volume(volume)
        world.add_phys(GDMLPhys(vol_name, position=position(z=location, unit="m")))
        down = up

    gdml.add_volume(world)
    return gdml


def save_full_atmosphere(path, with_spacecraft=False):
    gdml = create_full_earth_atmosphere(with_spacecraft)
    text = ET.tostring(gdml.gdml, encoding="utf-8")
    print(text)
    tree = etree.fromstring(text)
    with open(path, "w") as fout:
        fout.write(etree.tounicode(tree, pretty_print=True))
