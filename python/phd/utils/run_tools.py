import json
import logging
import os
import subprocess
from dataclasses import dataclass
from itertools import product
from multiprocessing import Pool
from string import Template
from timeit import default_timer as timer
from typing import Optional

import numpy as np
from dataforge import Meta, MetaRepr
import abc

logger = logging.getLogger(__name__)


def no_gdmL_input_generator(meta: Meta, macros_template: str):
    macros_template = Template(macros_template)
    for path, values in zip(
            dir_name_generator(".", "sim"),
            values_from_dict(meta)
    ):
        text = macros_template.substitute(values)
        data = InputData(
            text=text,
            path=path,
            values=Meta(values)
        )
        yield data


def general_input_generator(meta: Meta, gdml_template_file: str, macros_template: str):
    macros_template = Template(macros_template)

    paths, values_gdml = create_gdml(gdml_template_file, meta["gdml"])
    paths = list(map(lambda x: os.path.join("..", x), paths))
    meta["macros"]["path"] = paths
    for path, values in zip(
            dir_name_generator(".", "sim"),
            values_from_dict(meta["macros"])
    ):
        text = macros_template.substitute(values)
        path_gdml = values["path"]
        indx = paths.index(path_gdml)
        input_data_meta = {
            "macros": values,
            "gdml": values_gdml[indx]
        }
        data = InputData(
            text=text,
            path=path,
            values=Meta(input_data_meta)
        )
        yield data


class GdmlGenerator(abc.ABC):
    @abc.abstractmethod
    def generate(self, template_file):
        pass


def input_generator_custom_gdml(meta: Meta, gdml_template_file: str, macros_template: str,
                                gdml_generator: GdmlGenerator):
    paths, values_gdml = gdml_generator.generate(gdml_template_file)
    paths = list(map(lambda x: os.path.join("..", x), paths))
    meta["macros"]["path"] = paths
    macros_template = Template(macros_template)
    for path, values in zip(
            dir_name_generator(".", "sim"),
            values_from_dict(meta["macros"])
    ):
        text = macros_template.substitute(values)
        path_gdml = values["path"]
        indx = paths.index(path_gdml)
        input_data_meta = {
            "macros": values,
            "gdml": values_gdml[indx]
        }
        data = InputData(
            text=text,
            path=path,
            values=Meta(input_data_meta)
        )
        yield data


def create_gdml(template_file, values: dict):
    values = values_from_dict(values)
    os.makedirs("./gdml", exist_ok=True)
    paths = []
    values = list(values)
    with open(template_file) as fin:
        gdml_template = fin.read()
        # gdml_template = Template(gdml_template)
    for indx, value in enumerate(values):
        path = os.path.join("./gdml", "{}.gdml".format(indx))
        paths.append(path)
        create_one_file(gdml_template, path, value)
    return paths, list(values)


def create_one_file(text, foutput, values: dict):
    template = Template(text)
    os.makedirs(os.path.split(foutput)[0], exist_ok=True)
    with open(foutput, 'w') as fout:
        fout.write(template.safe_substitute(values))
    return foutput


from tqdm import tqdm


def values_from_dict(values: dict):
    keys, product_ = meta_analysis(values)
    n = len(product_)
    with tqdm(total=n) as pbar:
        for values in product_:
            pbar.update(1)
            yield {
                key: value for key, value in zip(keys, values)
            }


def meta_analysis(values: dict):
    keys = values.keys()

    for key, item in values.items():
        if not isinstance(item, list) and not isinstance(item, np.ndarray):
            values[key] = [item]

    # if 'sync' in keys:
    #     listForProduct = []
    #     listForSyncProduct = []
    #     keysFP = []
    #     keysFSP = []
    #     for key in keys:
    #         if key != 'sync':
    #             if key in values['sync']:
    #                 listForSyncProduct.append(values[key])
    #                 keysFSP.append(key)
    #             else:
    #                 listForProduct.append(values[key])
    #                 keysFP.append(key)
    #     productFP = list(product(*listForProduct))
    #     product_ = []
    #     logging.debug('Product for no sync: {}'.format(productFP))
    #     logging.debug('List for product sync: {}'.format(listForSyncProduct))
    #     for arg in zip(*listForSyncProduct):
    #         temp = list(arg)
    #         for pFP in productFP:
    #             product_.append(list(pFP) + temp)
    #     keys = keysFP + keysFSP
    # else:
    listForProduct = [values[key] for key in keys]
    product_ = product(*listForProduct)

    keys = list(keys)
    product_ = list(product_)

    logging.debug("".format(keys))
    for p in product_.copy():
        logging.debug("Values: {}".format(' '.join(map(str, p))))

    return keys, product_


def dir_name_generator(path, prefix, start=0):
    count = start
    while True:
        name = prefix + str(count).rjust(4, '0')
        if os.path.exists(name):
            count += 1
            continue
        yield name
        count += 1

    # dirs = os.listdir(path)
    # n = len(prefix)
    # prefixDirs = []
    # numbers = []
    # for dir_ in dirs:
    #     if dir_[:n] == prefix:
    #         postfix = dir_[n:]
    #         try:
    #             number = int(postfix)
    #             prefixDirs.append(dir_)
    #             numbers.append(number)
    #         except Exception:
    #             pass
    # try:
    #     max_ = max(numbers)
    # except Exception:
    #     max_ = 0
    # while True:
    #     max_ += 1
    #     yield prefix + str(max_).rjust(4, '0')


def run_command(parameters):
    input_data, command = parameters
    run_path = input_data.path
    os.makedirs(run_path, exist_ok=True)
    pwd = os.getcwd()
    os.chdir(run_path)

    logname = os.path.split(run_path)[-1]
    logger: logging.Logger = logging.getLogger(logname)
    logger.addHandler(
        logging.FileHandler("run_tools.log")
    )

    start = timer()
    p = subprocess.Popen(command, shell=True,
                         stdout=subprocess.PIPE,
                         stdin=subprocess.PIPE,
                         stderr=subprocess.PIPE,
                         encoding='utf-8'
                         )
    out, err = p.communicate(input_data.text)
    logger.debug(out)
    logger.debug(err)
    p.wait()
    end = timer()
    os.chdir(pwd)
    input_data.values["time"] = end - start

    return input_data


@dataclass
class InputData(MetaRepr):
    path: str
    text: str
    values: Optional[Meta] = None

    def to_meta(self) -> 'Meta':
        return Meta({
            "path": self.path,
            "text": self.text,
            "values": self.values
        })


def multirun_command(input_data_generator, command, n_cpu_cores=None, post_processor=None):
    if n_cpu_cores is None: n_cpu_cores = os.cpu_count()
    with Pool(n_cpu_cores) as p:
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


@dataclass
class CinServerParameters:
    command: str


class G4CinServer:
    def __init__(self, parameters: CinServerParameters):
        self.parameters = parameters
        self.is_start = False

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.stop()

    def start(self, text: str):
        command = self.parameters.command
        logger.info("Start cin server")
        self.process = subprocess.Popen(command, shell=True,
                                        # stdout=subprocess.PIPE,
                                        stdin=subprocess.PIPE,
                                        # stderr=subprocess.PIPE,
                                        encoding='utf-8'
                                        )
        self._write(text)
        self.is_start = True

    def _write(self, text):
        self.process.stdin.write(text)
        self.process.stdin.flush()

    def send(self, text: str):
        self._write(text)

    def stop(self):
        logger.info("Stop cin server")
        self._write("exit\n")
        exit = self.process.wait()
        self.is_start = False
        return exit
