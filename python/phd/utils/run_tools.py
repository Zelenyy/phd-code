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

GEANT4_OUTFILE = ['gdml/default.gdml', 'gps/gps.mac', 'mac/init.mac']


def create_one_file(finput, foutput, dict_value):
    with open(finput, 'r') as fin:
        text = fin.read()
    template = Template(text)
    os.makedirs(os.path.split(foutput)[0], exist_ok=True)
    with open(foutput, 'w') as fout:
        fout.write(template.safe_substitute(dict_value))
    return True


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


def run_command(parametres):
    inputData, command = parametres
    run_path = inputData.path
    os.chdir(run_path)
    p = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stdin=subprocess.PIPE)
    p.stdin.write(inputData.text)
    # p.stdin.flush()
    logging.debug(p.stdout.read().decode('utf-8'))
    p.wait()
    return run_path


@dataclass
class InputData:
    path : str
    text : str

def multirun_command(inputDataGenerator, command, n_cpu_cores = None, post_processor = None):
    if n_cpu_cores is None: n_cpu_cores= os.cpu_count()
    with Pool(n_cpu_cores)as p:
        logging.debug("Start multirun")
        for path in p.imap_unordered(run_command, [(inputData, command) for inputData in inputDataGenerator]):
            logging.debug("End run in path: " + path)
            if post_processor is not None:
                post_processor(path)
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

