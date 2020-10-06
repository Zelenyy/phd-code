import os

import star
import numpy as np
import tables
from phd.thunderstorm import atmosphere
import matplotlib.pyplot as plt

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



def get_group(path):
    with tables.open_file(path) as h5file:
        result = {}
        for group in h5file.root:
            table = h5file.get_node(group, "stacking_simple")
            # data = table.read()
            field = table.attrs["values_gdml_field"][0]
            height = table.attrs["values_gdml_height"][0]
            key = height
            if key in result.keys():
                result[key].append((field, group._v_name))
            else:
                result[key] = [(field, group._v_name)]
        for value in result.values():
            value.sort(key=lambda x: x[0])
        return result


def plot_minimal_field_production(path, output="plot"):

    if not os.path.exists(output):
        os.mkdir(output)

    groups = get_group(path)
    bins = np.arange(-500.0, 501, 1)
    x = bins[:-1]
    result = []
    dtype = np.dtype(
        [
            ("field", "d"),
            ("height", "d"),
            ("energy", "d"),
            ("k", "d"),
            ("b", "d")
        ]
    )

    with tables.open_file(path) as h5file:
        for height, value in groups.items():
            plt.clf()
            for field, group_name in value:
                table: tables.Table = h5file.get_node("/{}".format(group_name), "stacking_simple")
                data = table.read()
                number = table.attrs["values_macros_number"]
                temp, _ = np.histogram(data["z"], bins=bins)
                temp = np.cumsum(temp[::-1])
                y = temp / number
                plt.plot(x, y, label="{:.2f}kV/m".format(field*1e4))
            path = os.path.join(output, "{}m.png".format(height))
            plt.xlabel("Height, meters")
            plt.ylabel("Cumulative number of electron")
            plt.legend()
            plt.tight_layout()
            plt.savefig(path, format="png", transparent=True, dpi = 600)
    return 0

