from multiprocessing import Pool
import os
import pickle

from phd.thunderstorm.electric_field import generate_potential


def save_potential(filename):
    res = generate_potential()
    with open(filename ,'wb') as fout:
        pickle.dump(res, fout)
    return filename


def main():
    namelist = ['../../data/thunderstorm/random_field/potentialINR{}.obj'.format(str(i).rjust(3,'0')) for i in range(1,5)]
    with Pool(os.cpu_count()) as p:
        for name in p.imap_unordered(save_potential, namelist):
            print(name)


if __name__ == '__main__':
    main()