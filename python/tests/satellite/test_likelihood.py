from unittest import TestCase
import numpy as np
import tables
from phd.satellite.single_processing import load_splitting_likelihood_factory, NormilizerContainer
from phd.satellite.processing.single_processing import DetectorCharacter, SingleProcessing
import matplotlib.pyplot as plt


class Likelihood(TestCase):

    def test_likelihood_proton(self):
        path = "/home/zelenyy/data/satellite/mean_mesh.hdf5"
        splitting = [4 for i in range(20)]
        lh_split = load_splitting_likelihood_factory(path, particle="proton", splitting=splitting)
        detector = DetectorCharacter(aperture=30, proton_treshold=4.0, proton_high=100)
        # event = np.array([lh_split.interpolators_mean[i]([0.5, 0.0, 0.0])[0] for i in range(20)])
        event = np.array([lh_split.interpolators_mean[i]([0.7, 0.6, 0.0])[0] for i in range(20)])
        sp = SingleProcessing(lh_split, detector)
        print(sp.process(event))

    def test_likelihood_theta(self):
        path = "/home/zelenyy/data/satellite/mean_mesh.hdf5"
        splitting = [4 for i in range(20)]
        lh_split = load_splitting_likelihood_factory(path, particle="proton", splitting=splitting)
        event = np.array([lh_split.interpolators_mean[i]([0.7, 0.6, 0.0])[0] for i in range(20)])
        likelihood = lh_split.build(event)
        lh_grid = []
        x = np.linspace(0.0, 1.0, 300)
        for i in x:
            lh_grid.append(likelihood(np.array([0.7, i, 0.0])))
        lh_grid = np.array(lh_grid)
        plt.plot(lh_grid)
        plt.show()

    def test_reconstruction(self):
        path_mesh =  "/home/zelenyy/data/satellite/mean_mesh.hdf5"
        path_recon= "/home/zelenyy/npm/phd/phd-code/python/simulation_scripts/satellite/recon_3.hdf5"
        with tables.open_file(path_recon) as h5file:
            data_recon = h5file.get_node("/", "proton_mean_test").read()
        norm_cont = NormilizerContainer.load(path_mesh, particle="proton")
        data_recon["reconstructed"][:, 0] = norm_cont.energy_normilizer.unnormalize(data_recon["reconstructed"][:, 0])
        data_recon["reconstructed"][:, 1] = norm_cont.theta_normilizer.unnormalize(data_recon["reconstructed"][:, 1])
        data_recon["reconstructed"][:, 2]= norm_cont.shift_normilizer.unnormalize(data_recon["reconstructed"][:, 2])
        indx = data_recon["success"]
        data_recon = data_recon[indx]
        plt.clf()
        plt.scatter(data_recon["original"][:, 0], data_recon["reconstructed"][:, 0])
        plt.show()
        plt.clf()
        plt.scatter(data_recon["original"][:, 1], data_recon["reconstructed"][:, 1])
        plt.show()
        plt.clf()
        plt.scatter(data_recon["original"][:, 2], data_recon["reconstructed"][:, 2])
        plt.show()

    def test_chi2(self):
        path_mesh =  "/home/zelenyy/data/satellite/mean_mesh.hdf5"
        path_recon= "/home/zelenyy/npm/phd/phd-code/python/simulation_scripts/satellite/recon_3.hdf5"
        with tables.open_file(path_recon) as h5file:
            data_recon = h5file.get_node("/", "proton_mean_test").read()
