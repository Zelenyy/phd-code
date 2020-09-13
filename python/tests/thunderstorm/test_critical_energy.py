from unittest import TestCase

import numpy as np
from phd.thunderstorm.critical_energy import get_critical_energy
import matplotlib.pyplot as plt

class Test(TestCase):
    def test_critical_energy(self):
        heigth = 0.0
        fields = np.arange(1,11)
        critical_energy = []
        for field in fields:
            temp = get_critical_energy(heigth, field)
            if temp is None:
                print("Small field", field)
                critical_energy.append(temp)
            else:
                if not temp.converged:
                    print("Not convereged", heigth, field)
                critical_energy.append(temp.root)
        plt.plot(fields, critical_energy)
        plt.show()