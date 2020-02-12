import json
import os
from typing import Optional

import numpy as np
from dataforge import Meta
from tables import File, open_file, Group


def get_attrs_values(filename: str, attrs_name: str):
    result  = set()
    with open_file(filename) as h5file:
        for node in h5file.walk_nodes():
            if isinstance(node, Group):
                continue
            try:
                result.add(node.attrs[attrs_name])
            except (AttributeError, KeyError):
                continue
    return result

def find_group_by_meta(filename, **kwargs):
    result = []
    with open_file(filename) as h5file:
        for group in h5file.root:
            meta = h5file.get_node(group, "meta")
            meta = Meta(meta)
            flag = True
            for key, value in kwargs:
                if meta[key] != value:
                    flag = False
                    break
            if flag:
                result.append(group._v_pathname)
    return result


def find_by_meta(filename: str, target_node = None, meta: Optional[Meta] = None, **kwargs):

    def check(node, meta):
        for key, value in meta.items():
            if (node.attrs[key] != value):
                return False
        return True

    results = []
    with open_file(filename) as h5file:
        for group in h5file.root:
            if target_node is None:
                for node in h5file.list_nodes(group):
                    if check(node, kwargs):
                         results.append(node._v_pathname)
            elif isinstance(target_node, str):
                node_item = h5file.get_node(group, target_node)
                if check(node_item, kwargs):
                    results.append(node_item._v_pathname)
            elif isinstance(target_node, list):
                for node_item in node:
                    node_item = h5file.get_node(group, node_item)
                    if check(node_item, kwargs):
                         results.append(node_item._v_pathname)
    return results

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