from unittest import TestCase
import numpy as np
from phd.satellite.mean_table import MeanItem


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