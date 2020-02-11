import numpy as np
from scipy.optimize import curve_fit


def power(x, l):
    return np.exp(x/l)

def calc_gurevich_len_1(data, z_cut):

    data_e = data[data["particle"] == 11] # choose electrons
    data_e = data_e[np.logical_and(data_e["z"]> -z_cut, data_e["z"]<z_cut)]
    indx  =  np.isin(data_e["id"], data["parent_id"])
    data_e = data_e[indx]

    z = np.sort(data_e["z"])
    Ne = np.arange(1,z.size+1)


    popt, pcov = curve_fit(power, z, Ne, sigma=Ne**0.5)
    return popt, pcov


