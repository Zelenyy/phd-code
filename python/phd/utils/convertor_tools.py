import numpy as np


def direction_to_degree(direction):
    vector = np.fromstring(direction, sep=" ")
    return np.rad2deg(np.arccos(vector[-1]))