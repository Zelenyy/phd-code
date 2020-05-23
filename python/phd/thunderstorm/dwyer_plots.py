import matplotlib.pyplot as plt
import numpy as np


def polar_theta_energy(data, boundaries = (0, np.inf), file=None):
    n = len(boundaries)
    nrows, ncols = (n+1) // 2, 2
    plt.subplots(ncols=ncols, nrows=nrows, figsize=(5*ncols, 5*nrows))
    for i in range(n):
        if i!=n-1:
            indx = np.logical_and(data["energy"] > boundaries[i], data["energy"] < boundaries[i+1])
            label = "{}<energy<{}".format(boundaries[i], boundaries[i+1])
        else:
            indx = data["energy"] > boundaries[i]
            label = "energy>{}".format(boundaries[i])
        ax = plt.subplot(nrows,ncols, i+1, projection='polar')
        ax.set_title(label)
        ax.plot(data["theta"][indx], data["energy"][indx], ".")
        ax.set_theta_zero_location("N")  # theta=0 at the top
        ax.set_theta_direction(-1)
        ax.set_thetalim(0, np.pi)
