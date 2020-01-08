from string import Template
import sys, os
import shutil
import subprocess
import logging
import json, time
from multiprocessing import Pool
from itertools import product
from functools import partial

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


def meta_analysis(dict_value: dict):
    keys = dict_value.keys()

    for key, value in dict_value.items():
        if type(value)!= list:
            dict_value[key] = [value]

    if 'sync' in keys:
        listForProduct = []
        listForSyncProduct = []
        keysFP = []
        keysFSP = []
        for key in keys:
            if key!='sync':
                if key in dict_value['sync']:
                    listForSyncProduct.append(dict_value[key])
                    keysFSP.append(key)
                else:
                    listForProduct.append(dict_value[key])
                    keysFP.append(key)
        productFP = list(product(*listForProduct))
        product_ = []
        logging.debug('Product for no sync: {}'.format(productFP))
        logging.debug('List for product sync: {}'.format(listForSyncProduct))
        for arg in zip(*listForSyncProduct):
            temp = list(arg)
            for pFP in productFP:
                product_.append(list(pFP) + temp)
        keys = keysFP + keysFSP
    else:
        listForProduct = [dict_value[key] for key in keys]
        product_ = product(*listForProduct)

    keys  = list(keys)
    product_ = list(product_)

    logging.debug("".format(keys))
    for p in product_.copy():
        logging.debug("Values: {}".format(' '.join(map(str,p))))

    return keys, product_

def create_files(infiles, outfiles, simulationDir, dict_value, prefix='sim'):
        keys, product_ = meta_analysis(dict_value)
        os.makedirs(simulationDir, exist_ok=True)
        with open(os.path.join(simulationDir, 'run_tools.log'), 'a') as flog:
            flog.write(time.strftime("[%a, %d %b %Y %H:%M:%S] ", time.gmtime()))
            flog.write(' '.join(keys))
            flog.write(' ' + str(dict_value) + '\n')
        paths = []
        logging.debug('Start to create files')
        for values, name_dir in zip(product_, dir_name_generator(simulationDir, prefix)):
            temp_dict = {key: value for key, value in zip(keys, values)}
            name_dir = os.path.join(simulationDir, name_dir)
            paths.append(name_dir)
            os.makedirs(name_dir)
            logging.debug('Paths for simulations: {}'.format(name_dir))
            with open(os.path.join(name_dir, 'meta.json'), 'w') as fmeta:
                json.dump(temp_dict, fmeta)
            for infile, outfile in zip(infiles, outfiles):
                foutput = os.path.join(name_dir, outfile)
                create_one_file(infile, foutput, temp_dict)

        return paths

def create_configure(paths, **kwargs):
    for path in paths:
        with open(os.path.join(path,'configure.txt'), 'w') as fout:
            for key, value in kwargs.items():
                fout.write('='.join(map(str,[key,value])))
                fout.write('\n')


def run_command(run_path, command):
    # pwd = os.getcwd()
    os.chdir(run_path)
    sys.stdout = open('stdout.txt', 'w')
    p = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE)
    logging.debug(p.stdout.read().decode('utf-8'))
    p.wait()
    # os.chdir(pwd)
    return run_path

def multirun_command(paths, command, n_cpu_cores = None):
    if n_cpu_cores is None: n_cpu_cores= os.cpu_count()
    with Pool(n_cpu_cores)as p, open('multirunCommand.log', 'a') as flog:
        flog.write(time.strftime("[%a, %d %b %Y %H:%M:%S] ", time.gmtime()) + "start multirun\n")
        for path in p.imap_unordered(partial(run_command, command=command), [os.path.join(os.getcwd(), path) for path in paths]):
            flog.write(time.strftime("[%a, %d %b %Y %H:%M:%S] ", time.gmtime()) + path+'\n')
        flog.write(time.strftime("[%a, %d %b %Y %H:%M:%S] ", time.gmtime()) + "end multirun\n")
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

