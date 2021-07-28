from unittest import TestCase
import numpy as np
from phd.satellite.mean_table import NormilizerContainer
from phd.satellite.single_processing import DataMeshLoader, calculate_interpolators
from scipy.stats import describe


class TestSingleProcessing(TestCase):


    def setUp(self) -> None:
        self.mesh_path = "/home/zelenyy/backup_data/data/satellite/polistyrene/mean_mesh.hdf5"
        self.proton_path = "/home/zelenyy/backup_data/data/satellite/polistyrene/proton.hdf5"

    def test_check_normilizer(self):
        contenier = NormilizerContainer.load(self.mesh_path, particle="proton")
        energy = np.array([10, 20, 30, 40])
        normed = contenier.energy_normilizer.normalize(energy)
        print(normed)
        renormed = contenier.energy_normilizer.unnormalize(normed)
        self.assertTrue(np.allclose(energy, renormed))

    def test_mesh_loader(self):
        data_mesh_loader = DataMeshLoader.load(self.mesh_path, particle="proton")
        print(describe(data_mesh_loader.energy))
        print(describe(data_mesh_loader.theta))
        print(describe(data_mesh_loader.shift))
        print(data_mesh_loader.mean.shape)

    def test_interpolatos(self):
        data_mesh_loader = DataMeshLoader.load(self.mesh_path, particle="proton")
        split_mean_mesh = data_mesh_loader.mean
        split_var_mesh = data_mesh_loader.var
        normalizers = data_mesh_loader.normalizers
        inter_list, inter_std_list = calculate_interpolators(
            data_mesh_loader.energy,
            data_mesh_loader.theta,
            data_mesh_loader.shift,
            split_mean_mesh,
            split_var_mesh)
        points = data_mesh_loader.get_normed_points()
        inter = inter_list[0]
        data = data_mesh_loader.mean[0].ravel()
        result = inter(points)

        self.assertTrue(np.allclose(data, result))





