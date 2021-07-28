from dataclasses import dataclass

import numpy as np


@dataclass
class SCRProton:
    C: float
    ga: float
    gb: float
    E0: float
    dt : float
    event : str = ""


def scr_proton_spectrum(E : np.ndarray, parametes: SCRProton):
    ga = parametes.ga
    gb = parametes.gb
    E0 = parametes.E0
    C = parametes.C
    temp = (gb-ga)*E0
    result = np.zeros(E.size, "d")
    indx = E > temp
    result[indx]  = C * (E[indx] ** (- gb)) * (((gb - ga) * E0) ** (gb - ga)) * np.exp(ga - gb)
    indx = np.logical_not(indx)
    result[indx] = C * E[indx] ** (-ga) * np.exp(-E[indx] / E0)
    return result



def get_parameters():
    return [
        SCRProton(
            C = 3.26 * 10 ** 7,
            ga = 0.87,
            gb = 4.68,
            E0 = 12.8,
            dt = 1.44 * 10 ** 5,
            event= "26/10/03"
        ),
        SCRProton(
            C=1.35 * 10 ** 9,
            ga=1.04,
            gb=3.52,
            E0=27.4,
            dt=1.19 * 10 ** 5,
            event="28/10/03"
        ),
        SCRProton(
            C=3.05 * 10 ** 8,
            ga=1.10,
            gb=3.15,
            E0=26.1,
            dt=1.84 * 10 ** 5,
            event="29/10/03"
        ),
        SCRProton(
            C=4.89 * 10 ** 8,
            ga=1.09,
            gb=3.44,
            E0=13.2,
            dt=1.87 * 10 ** 5,
            event="02/11/03"
        ),
        SCRProton(
            C= 1.4 * 10 ** 8,
            ga= 1.52,
            gb= 4.86,
            E0= 21.7,
            dt= 2.27 * 10 ** 5,
            event="04/11/03"
        ),

    ]

@dataclass
class SCRElectron:
    Ca : float
    ga : float
    Cb : float
    gb : float
    dt : float
    event: str = ""


def scr_electron_spectrum(E : np.ndarray, parametes: SCRElectron):
    ga = parametes.ga
    gb = parametes.gb
    Ca = parametes.Ca
    Cb = parametes.Cb

    result = np.zeros(E.size, "d")
    indx = E > 1.32
    result[indx]  = Cb * (E[indx] ** (- gb))
    indx = np.logical_not(indx)
    result[indx] = Ca * E[indx] ** (-ga)
    return result

def load_electron_spectrum(path):
    result = []
    with open(path) as fout:
        for line in fout.readlines():
            line = line.split(" ")
            # print(line)
            event = line[0]
            Ca, ga, Cb, gb, dt = tuple(map(float, line[1:]))
            result.append(SCRElectron(Ca, ga, Cb, gb, dt, event))
    return result