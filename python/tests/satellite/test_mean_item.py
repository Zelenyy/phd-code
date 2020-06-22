from unittest import TestCase
import numpy as np
from phd.satellite.mean_table import MeanItem
from phd.satellite.single_processing import DataMeshLoader


class TestMeanItem(TestCase):

    def setUp(self) -> None:
        n = 500
        data1 = np.random.random_sample(size=(n,100))
        self.item1 = MeanItem(mean=data1.mean(axis=0), variance=data1.var(axis=0), number=n)

        data2 = np.random.random_sample(size=(n,100))
        self.item2 = MeanItem(mean=data2.mean(axis=0), variance=data2.var(axis=0),number=n)

        data = np.vstack((data1, data2))
        self.item = MeanItem(mean=data.mean(axis=0), variance=data.var(axis=0),number=2*n)

    def test_join_item(self):
        item = MeanItem.join_item(self.item1, self.item2)
        self.assertAlmostEqual(item.mean[0], self.item.mean[0])
        self.assertAlmostEqual(item.variance[0], self.item.variance[0], places=3)


    def test_sum_real_data(self):
        path = "/home/zelenyy/data/satellite/mean_mesh.hdf5"
        dataLoaderProton = DataMeshLoader(path, particle="proton")
        print(dataLoaderProton.mean[:, 0,0,0].sum())
        mean = MeanItem.sum_item(*[MeanItem(dataLoaderProton.mean[i], dataLoaderProton.var[i], number=1) for i in range(dataLoaderProton.mean.shape[0])])
        print(mean.mean[0, 0, 0].sum())
        self.assertAlmostEqual(mean.mean[0, 0, 0].sum(), dataLoaderProton.mean[:, 0,0,0].sum())