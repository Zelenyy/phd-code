import numpy as np

import matplotlib.pyplot as plt
import numpy as np
import matplotlib as mpl

def read_kotlin_rltge_text_file(path):
    with open(path) as fin:
        text = fin.readlines()

    def split_line(line):
        temp = line.split(" ")
        return int(temp[6][:-1]), int(temp[2])

    data = np.array(list(map(split_line, text[:-1])))


def plot_data(data, show=False):
    mpl.rcParams['font.size'] = 14
    plt.plot(data[:, 0], data[:, 1], label="Gain=1.6")
    plt.ylabel("Number of gamma-quanta")
    plt.xlabel("Number of generation")
    plt.title("Interesing sample for 100 seed photons")
    plt.legend()
    if show: plt.show()