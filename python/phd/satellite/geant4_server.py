import os
import socket
import struct
import time
import logging
from dataclasses import dataclass
from enum import Enum
from string import Template
from typing import List, Callable, Union
import subprocess

from phd.satellite.mean_table import MeanTable
from phd.satellite.run import QueueData, request_generator
from phd.satellite.satellite_pb2 import MeanRun

INIT_TEMPLATE = Template(
"""/npm/geometry/type gdml
/npm/geometry/gdml ${gdml}
/npm/satellite/output socket
/npm/satellite/port ${port}
/npm/satellite/detector ${mode}
"""
)

SEPARATOR = b"separator\n"

class DetectorMode(Enum):
    SINGLE = "single"
    SUM = "sum"

class Geant4Server:
    def __init__(self, meta: dict):
        self.command = meta["command"]
        self.meta = meta

    def _start(self):
        """
        :param mode:
            Если mode =  DetectorMode.SINGLE то сервер возвращает данные пособытийно, то есть распредление энерговыделегний в детекторе для каждого отдельного события
            Если mode =  DetectorMode.SUM то сервер будет возвращать сумарное энерговыделение за сеанс от всех событий
        :return:
        """
        logging.info("Start server: {}".format(self.command))
        self.process = subprocess.Popen(self.command,
                                        shell=True,
                                        stdin=subprocess.PIPE,
                                        stdout=subprocess.PIPE)

        text : str = INIT_TEMPLATE.substitute(
            self.meta
        )
        self._write(text)
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        while True:
            try:
                data_host = '127.0.0.1'
                self.socket.connect((data_host, self.meta["port"]))
                break
            except Exception:
                # print("sleep")
                time.sleep(0.1)
        return 0

    def _write(self, text):
        self.process.stdin.write(text.encode())
        self.process.stdin.write(SEPARATOR)
        self.process.stdin.flush()

    def send(self, text: str) -> bytes:
        """

        :param text: тескт сообщения посылаемого  на сервер
        :param data_host: адресс хоста на котором сервер будет возвращать даные
        :param data_port: порт через который сервер будет возращать данные
        :return: Run --- десериализованный protbuff
        """
        self._write(text)
        logging.info("Send request")
        ultimate_buffer = b''
        size = self.socket.recv(8)
        size = struct.unpack("@L", size)[0]
        # print(size)
        ultimate_buffer = self.socket.recv(size)
        return ultimate_buffer

    def __enter__(self):
        self._start()
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.stop()
        if exc_val:
             raise

    def stop(self):
        self.socket.close()
        self.process.stdin.write(b"exit\n")
        self.process.stdin.flush()
        self.process.wait()
        logging.info("Stop server")
        return 0

from typing import Generator



from multiprocessing import Process, Pipe, Queue, get_logger, log_to_stderr

@dataclass
class MessageParameters:
    name : str
    type : str


def server_run(meta_factory : Generator, values_macros: dict, parameters: MessageParameters, n_workers = None):
    if n_workers is None: n_workers = os.cpu_count()
    # logger = log_to_stderr(logging.INFO)
    input_queue = Queue(maxsize=2*n_workers)
    output_queue = Queue(maxsize=2*n_workers)
    requester = Process(target=generate_request, args=(n_workers, values_macros, input_queue))
    requester.start()
    workers = multythread_server(meta_factory, input_queue, output_queue, n_workers)

    processor = Process(target=process_message, args=(n_workers, output_queue, parameters))
    processor.start()
    # input_queue.join_thread()
    # requester.join()
    # processor.join()
    return 0



def generate_request(n_workers, values_macros: dict, input_queue: Queue):
    # logger = log_to_stderr(logging.INFO)
    for indx, data in enumerate(request_generator(values_macros, [0.0, 0.0, 0.1])):
        input_queue.put(data)
        # logger.info("Put request number {}".format(indx))

    for i in range(n_workers):
        input_queue.put("END")
        # logger.info("Put END number {}".format(i))
    # input_queue.close()
    return 0


def process_message(n_workers, output_queue: Queue, parameters: MessageParameters):
    count = 0
    # logger = log_to_stderr(logging.INFO)
    # logger.info("Start process")
    if parameters.type == "mean":
        run = MeanRun()
    with MeanTable(parameters.name) as mean_table:
        while True:
            message = output_queue.get()
            if message == "END":
                count += 1
                # logger.info("Get END number {}".format(count))
                if count == n_workers:
                    break
                continue
            run.ParseFromString(message.data)
            mean_table.append_from_mean_run(run, message.meta)
    # output_queue.close()
    # output_queue.join_thread()
    return 0


def multythread_server(meta_factory : Generator, input_queue: Queue, output_queue: Queue, n_workers):
    workers = []

    for i in range(n_workers):
        meta = next(meta_factory)
        worker = Process(target=start_server_in_thread, args=(meta, input_queue, output_queue))
        worker.start()
        workers.append(worker)
    return workers


def start_server_in_thread(meta, input_queue: Queue, output_queue: Queue):
    # logger = log_to_stderr(logging.INFO)
    # logger.info("Start worker")
    with Geant4Server(meta) as server:
        for input_data in iter(input_queue.get, "END"):
            text = input_data.data
            # logger.info(input_data)
            meta = input_data.meta
            data = server.send(text)
            output_queue.put(QueueData(meta, data))
    output_queue.put("END")
    return 0




    # temp = 0
    # with Geant4Server(["../build/satellite/geant4-satellite.exe server"], parameters.meta) as server:
    #     run = parameters.parser_factory()
    #     for text, value in parameters.generator:
    #         data = server.send(text)
    #         run.ParseFromString(data)
    #         for event in run.event:
    #             temp += event.deposit[0]
    # return temp
