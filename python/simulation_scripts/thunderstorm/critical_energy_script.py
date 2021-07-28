from phd.thunderstorm import atmosphere
from phd.thunderstorm.critical_energy import get_critical_energy
import matplotlib.pyplot as plt
import numpy as np
from tabulate import tabulate

def main():
    heights = np.arange(21)*1000.0
    fields = np.arange(0.1, 11, 0.1)
    dtype = np.dtype([
        ("field", "d"),
        ("height", "d"),
        ("critical_energy", "d")
    ])
    data = np.zeros(heights.size*fields.size, dtype=dtype)
    indx = 0
    for height in heights:
        for field in fields:
            data["field"][indx] = field
            data["height"][indx] = height
            temp = get_critical_energy(height, field)
            if temp is None or not temp.converged:
                data["critical_energy"][indx] = float('nan')
            else:
                data["critical_energy"][indx] = temp.root
            indx += 1

    print(tabulate(data, headers=dtype.names))
    with open("critical_energy.csv", "w") as fout:
        res = tabulate(data, headers=dtype.names, tablefmt="plain")
        fout.write(res)
    np.save("critical_energy.npy", data)
    return 0


if __name__ == '__main__':
    main()