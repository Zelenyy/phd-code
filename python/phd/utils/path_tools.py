import json
import os

import numpy as np


def read_meta(path_dir):
    with open(os.path.join(path_dir, 'meta.json')) as fin:
        return json.load(fin)


def find_meta_path(root_path):
    paths = []
    for pathDir, dirList, fileList in os.walk(root_path):
        if 'meta.json' in fileList:
            paths.append(pathDir)
    return paths


def select_meta_path(paths, **kwargs):
    newPaths = []
    keys = set(kwargs.keys())
    for path in paths:
        meta = read_meta(path)
        temp = False
        mkeys = set(meta.keys())
        for key in keys:
            if not (key in mkeys):
                break
        else:
            temp = True
        if not temp:
            continue
        for key in keys:
            if type(kwargs[key]) == list:
                if not meta[key] in kwargs[key]:
                    temp = False
            elif meta[key] != kwargs[key]:
                temp = False
        if temp:
            newPaths.append(path)
    return newPaths


class LogTime:
    """
    Сканирует логи симуляций в заданной директории и находит время каждой симуляции
    """
    def __init__(self, init_path):
        self.time, self.paths = self.grep_time(init_path)

    def grep_time(self, init_path):
        time = []
        paths = []
        for path in os.walk(init_path):
            for file in path[2]:
                if file == 'log.txt':
                    paths.append(path[0])
                    with open(os.path.join(path[0], file)) as fin:
                        lines = fin.readlines()
                        temp = lines[-2].split()
                        time.append(int(temp[-1]))
        return np.array(time), paths


    def print_time_stat(self):
        time = self.time
        print("Number of files: {}".format(time.shape[0]))
        print("Max: {}".format(time.max()))
        print("Min: {}".format(time.min()))
        print("Average: {}".format(time.mean()))
        print("Full time, hours: {}".format(time.sum() / 3600))

    def print(self):
        for t, p in zip(self.time, self.paths):
            print("{} : {}".format(p, t))