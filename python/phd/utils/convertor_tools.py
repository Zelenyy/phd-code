import numpy as np


def direction_to_degree(direction):
    vector = np.fromstring(direction, sep=" ")
    return np.rad2deg(np.arccos(vector[-1]))


def theta_to_direction(theta):
    x = np.round(np.sin(theta), 3)
    y = 0
    z = np.round(np.cos(theta), 3)
    return f"{x} {y} {z}"
