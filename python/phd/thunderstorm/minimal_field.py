import os

import star
import numpy as np
import tables
from numpy.linalg import lstsq
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
            # plt.yscale("log")
            plt.tight_layout()
            plt.savefig(path, format="png", transparent=True, dpi = 600)
    return 0


def find_minimal_field(path):
    groups = get_group(path)
    bins = np.arange(-500.0, 501, 1)
    x = bins[:-1]
    M = x[:, np.newaxis] ** [0, 1]
    result = []
    dtype = np.dtype(
        [
            ("height", "d"),
            ("field", "d"),
            ("baseline", "d"),
            ("coverage", np.bool_)
        ]
    )

    with tables.open_file(path) as h5file:
        for height, value in groups.items():
            res_height = []
            baseline = get_minimal_field(height)
            for field, group_name in value:
                table: tables.Table = h5file.get_node("/{}".format(group_name), "stacking_simple")
                data = table.read()
                number = table.attrs["values_macros_number"]
                temp, _ = np.histogram(data["z"], bins=bins)
                temp = np.cumsum(temp[::-1])
                y = temp / number
                res_height.append((field, y))

            field, y = res_height[-1]
            if y[-1] <= 2:
                result.append((height, field, baseline, False))
            else:
                prev_field = res_height[-1][0]
                for field, y in res_height[::-1]:
                    p, res, rnk, s = lstsq(M, y)
                    k = p[1]
                    if k<0.001:
                        field = (field + prev_field)/2
                        result.append((height, field, baseline, True))
                        break
    return np.array(result, dtype=dtype)
