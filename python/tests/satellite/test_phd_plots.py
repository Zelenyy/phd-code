from unittest import TestCase

from phd.satellite.scr import get_parameters, scr_proton_spectrum, load_electron_spectrum, scr_electron_spectrum

import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
import os

class TestPhdPlots(TestCase):
    text_image_path = '/home/zelenyy/npm/another/scientific-papers/Russian-Phd-LaTeX-Dissertation/Dissertation/images/satellite/'


    def setUp(self) -> None:
        plt.clf()


    def set_text_parameters(self):
        mpl.rcParams.update(
            {'font.family': 'sans-serif',
             'font.size': 14,
             'axes.labelsize': 14,
             'xtick.labelsize': 14,
             'ytick.labelsize': 14,
             'legend.fontsize': 14})

    def test_text_proton_spectrum(self):
        self.set_text_parameters()
        scr_parameters = get_parameters()
        energy = np.linspace(0.1, 1000, num=300)
        for parameters in scr_parameters:
            data = scr_proton_spectrum(energy, parameters)
            plt.plot(energy, data / parameters.dt, label=parameters.event)
        plt.yscale("log")
        plt.xscale("log")
        plt.grid(True)
        plt.xlabel("Кинетическая энергия, МэВ")
        plt.ylabel(r"$\frac{Протон}{cm^2 \cdot c \cdot МэВ}$")
        plt.legend()
        plt.tight_layout()
        plt.savefig(os.path.join(self.text_image_path, "proton_spectrum.pdf"), format="pdf", transparent=True)

    def test_text_electron_spectrum(self):
        self.set_text_parameters()
        energy = np.linspace(0.01, 100, num=300)
        path = "/mnt/storage2/phd/data/satellite/Theory/electron_spectrum_jgr_110_2005.dat"
        electon_parameters = load_electron_spectrum(path)
        for parameters in electon_parameters:
            data = scr_electron_spectrum(energy, parameters)
            plt.plot(energy, data / parameters.dt, label=parameters.event)
        plt.yscale("log")
        plt.xscale("log")
        plt.grid(True)
        plt.xlabel("Кинетическая энергия, МэВ")
        plt.ylabel(r"$\frac{Электрон}{cm^2 \cdot c \cdot МэВ}$")
        plt.legend()
        plt.tight_layout()
        # plt.show()
        plt.savefig(os.path.join(self.text_image_path, "electron_spectrum.pdf"), format="pdf", transparent=True)