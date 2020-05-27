from dataclasses import dataclass

import numpy as np


@dataclass
class SCRParameters:
    C: float
    ga: float
    gb: float
    E0: float
    dt : float
    event : str = ""


def scr_proton_spectrum(E : np.ndarray, parametes: SCRParameters):
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
        SCRParameters(
            C = 3.05 * 10 ** 8,
            ga = 1.10,
            gb = 3.15,
            E0 = 26.1,
            dt = 1.84 * 10 ** 5,
            event= "29/10/03"
        )
    ]