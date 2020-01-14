from dataclasses import dataclass
from string import Template
import sys, os
import shutil
import subprocess
import logging
import json, time
from multiprocessing import Pool
from itertools import product
from functools import partial

from dataforge.meta import Meta


def create_from_file_template(fin, fout, values: dict):
    with open(fin) as fin:
        text = fin.read()
    return create_one_file(text, fout, values)


def create_one_file(text, foutput, values: dict):
    template = Template(text)
    os.makedirs(os.path.split(foutput)[0], exist_ok=True)
    with open(foutput, 'w') as fout:
        fout.write(template.safe_substitute(values))
    return foutput


def dir_name_generator(path, prefix):
        dirs = os.listdir(path)
        n = len(prefix)
        prefixDirs = []
        numbers = []
        for dir_ in dirs:
            if dir_[:n] ==prefix:
                postfix = dir_[n:]
                try:
                    number = int(postfix)
                    prefixDirs.append(dir_)
                    numbers.append(number)
                except Exception:
                    pass
        try:
            max_ = max(numbers)
        except Exception:
            max_ = 0
        while True:
            max_ += 1
            yield prefix + str(max_).rjust(4, '0')


def run_command(parameters):
    input_data, command = parameters
    run_path = input_data.path
    os.makedirs(run_path, exist_ok=True)
    os.chdir(run_path)
    p = subprocess.Popen(command, shell=True,
                         stdout=subprocess.PIPE,
                         stdin=subprocess.PIPE,
                         encoding='utf-8'
                         )
    out, err = p.communicate(input_data.text)
    logging.debug(out)
    p.wait()
    return input_data


@dataclass
class InputData:
    path : str
    text : str

def multirun_command(input_data_generator, command, n_cpu_cores = None, post_processor = None):
    if n_cpu_cores is None: n_cpu_cores= os.cpu_count()
    with Pool(n_cpu_cores)as p:
        logging.debug("Start multirun")
        for data in p.imap_unordered(run_command, [(inputData, command) for inputData in input_data_generator]):
            logging.debug("End run in path: " + data.path)
            if post_processor is not None:
                post_processor(data)
        logging.debug("End multirun")
    return True


def read_log(log: str):
    items = log.split()
    json_acceptable_string = items[-1].replace("'", "\"")
    dict_values = json.loads(json_acceptable_string)
    keys = items[-1:1]
    time = items[0]
    return time, keys, dict_values


def create_path(keys, dict_values):
    listForProduct = [dict_values[key] for key in keys]
    product_ = product(*listForProduct)
    paths = []
    for values in product_:
        name_dir = '_'.join(map(str, values))
    return paths

