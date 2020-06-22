from dataclasses import dataclass

import numpy as np
from phd.satellite.processing.likelihood_factory import LikelihoodFactory
from scipy.optimize import minimize


@dataclass
class DetectorCharacter:
    aperture : float = None
    proton_treshold : float  = 10.0
    proton_high : float = 120.0


class SingleProcessing:

    def __init__(self, likelihood_fact: LikelihoodFactory, detector: DetectorCharacter):
        self.likelihood_fact = likelihood_fact
        self.detector = detector

    # def process(self, event: np.ndarray, error: np.ndarray = None):
    #     likelyhood = self.likelihood_fact.build(event)
    #     # simplex_1 = ((likelyhood.full_energy, 0.0, 0.0), (), ())
    #     bounds = ((likelyhood.full_energy, 1.0), (0.0, likelyhood.max_theta), (0.0, 1.0))
    #     # x0 = np.array([likelyhood.full_energy, likelyhood.max_theta/2, 0.5])
    #     x0 = np.array([likelyhood.full_energy, 0.0, 0.5])
    #     result = minimize(likelyhood, x0, method="L-BFGS-B", bounds=bounds)
    #     return result
    #
    # def process(self,  event: np.ndarray):
    #     if np.all(event == 0):
    #         return None
    #     if np.all(event[1:] == 0):
    #         return self.process_only_first_layer(event[0])
    #     likelyhood = self.likelihood_fact.build(event)
    #     theta_min = likelyhood.max_minus_theta(0.0)
    #     theta_max = likelyhood.max_plus_theta(likelyhood.radius)
    #     bounds = ((likelyhood.full_energy, 1.0), (theta_min,theta_max), (0.0, 1.0))
    #     radius = likelyhood.radius
    #     min_range = likelyhood.min_range
    #     cons = ({"type" : "ineq", "fun" : lambda x : np.array([
    #         np.rad2deg(np.arctan((radius+x[2])/min_range)) - likelyhood.theta_normilizer.unnormalize(x[0]),
    #         likelyhood.theta_normilizer.unnormalize(x[0]) +  np.rad2deg(np.arctan((2*radius  - x[2])/min_range))
    #     ]) })
    #     x0 = np.array([likelyhood.full_energy, (theta_max+theta_min)/2.0, 0.5])
    #     result = minimize(likelyhood, x0, method="SLSQP", constraints=cons, bounds=bounds)
    #     return result

    def process(self, event: np.ndarray, error: np.ndarray = None):
        if np.all(event == 0):
            return None
        if np.all(event[1:] == 0):
            return self.process_only_first_layer(event[0])
        likelyhood = self.likelihood_fact.build(event)
        if likelyhood.min_range == 0:
            return None
        theta_min = likelyhood.max_minus_theta(0.0)
        theta_max = likelyhood.max_plus_theta(likelyhood.radius)
        bounds = ((likelyhood.full_energy, 1.0), (theta_min,theta_max), (0.0, 1.0))
        x0 = np.array([likelyhood.full_energy, (theta_max + theta_min) / 2.0, 0.5])
        # result = minimize(likelyhood, x0, method="Nelder-Mead")
        result = minimize(likelyhood, x0, method="L-BFGS-B", bounds=bounds)

        return result

    def process_only_first_layer(self, deposit: float):
        return None

    # def process(self, event: np.ndarray, error: np.ndarray = None):
    #     # print(event)
    #     if np.all(event == 0):
    #         return None
    #     if np.all(event[1:] == 0):
    #         return self.process_only_first_layer(event[0])
    #
    #     # full_energy = np.sum(event)
    #     #
    #     # indx = event != 0
    #     # if np.all(indx):
    #     #     min_energy = self.detector.proton_high
    #     # else:
    #     #     i = indx.argmin()
    #     #     event = event[:i]
    #     # bregg_ratio = event.max()/event.mean()
    #
    #     results = []
    #     likelyhood = self.likelihood_fact.build(event)
    #     # x0 = np.array([likelyhood.full_energy, likelyhood.max_theta/2, 0.5])
    #     for i in np.linspace(0, 1, 10):
    #         x0 = np.array([likelyhood.full_energy, likelyhood.max_theta / 2, i])
    #         result = minimize(likelyhood, x0, method="Nelder-Mead")
    #         results.append(result)
    #     return min(results, key=lambda x: x.fun)

    # def process(self, event: np.ndarray, error: np.ndarray = None):
    #     print(event)
    #     if np.all(event == 0):
    #         return None
    #     likelyhood = self.likelihood_fact.build(event)
    #     x0 = np.array([likelyhood.full_energy, likelyhood.max_theta/2, 0.5])
    #
    #     fe = likelyhood.full_energy
    #     mt = likelyhood.max_theta
    #
    #     A = (fe, mt, 0.0)
    #     B = (fe, 0.0, 0.0)
    #     C = (1.0,0.0,0.0)
    #     D = (1.0, mt, 0.0)
    #
    #     A1 = (fe, mt, 1.0)
    #     B1 = (fe, 0.0, 1.0)
    #     C1 = (1.0,0.0,1.0)
    #     D1 = (1.0, mt, 1.0)
    #
    #     simplexs = [
    #                 (A, B, B1, C),
    #                 (A, C, B1, D1),
    #                 (A, D, D1, C),
    #                 (A, A1, B1, D1),
    #                 (C, B1, C1, D1)]
    #     results = []
    #     for simplex in simplexs:
    #         result = minimize(likelyhood, x0, method="Nelder-Mead", options={"initial_simplex" : np.array(simplex)})
    #         results.append(result)
    #
    #     return min(results, key = lambda x: x.fun)