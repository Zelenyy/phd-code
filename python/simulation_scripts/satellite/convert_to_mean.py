from phd.satellite.mean_table import MeanTable


def main():
    MeanTable.collect_mean(
        "mean_anthracene.hdf5",
        "mcmc_1_69.hdf5",
        "mcmc_70_99.hdf5",
        "mcmc_100_119.hdf5",
        "mcmc_120_139.hdf5",
        "mcmc_140_150.hdf5",
                           )


if __name__ == '__main__':
    main()