from unittest import TestCase

import numpy as np
import star
from phd.thunderstorm import atmosphere
from phd.thunderstorm.critical_energy import get_critical_energy, calculate_secondary_production_rate
import matplotlib.pyplot as plt
from tabulate import tabulate


class Test(TestCase):
    def test_critical_energy(self):
        height = 0.0
        fields = np.arange(1,11)
        critical_energy = []
        for field in fields:
            temp = get_critical_energy(height, field)
            if temp is None:
                print("Small field", field)
                critical_energy.append(temp)
            else:
                if not temp.converged:
                    print("Not convereged", height, field)
                critical_energy.append(temp.root)

        material = star.electron.PredefinedMaterials.AIR_DRY_NEAR_SEA_LEVEL
        density = atmosphere.ISACalculator.density(height)
        data = star.electron.calculate_estar_table(material)

        energy = data["energy"]
        indx = (energy < 0.5) * (energy > 0.001)
        energy = energy[indx]
        stopPower = data["stopping_power_total"][indx]*density

        plt.plot(energy, stopPower)
        n = len(energy)

        for field, ce in zip(fields, critical_energy):
            if ce is not None:
                plt.hlines(field, energy.min(), energy.max(), "k", linestyles="--")
                plt.vlines(ce, 0, 15, "r", linestyles="--",)

        plt.show()

    def test_calculate_secondary_production_rate(self):
        path = "/mnt/storage2/phd/data/thunderstorm/critical_energy/result.hdf5"
        data = calculate_secondary_production_rate(path)
        np.save("rate.npy", data)
        print(tabulate(data, headers=data.dtype.names, tablefmt="plain"))