import os
import socket
import struct
import time
import logging
from dataclasses import dataclass
from enum import Enum
from string import Template
import subprocess
from typing import Generator
from multiprocessing import Process, Queue

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
                time.sleep(0.1)
        return 0

    def _write(self, text):
        self.process.stdin.write(text.encode())
        self.process.stdin.write(SEPARATOR)
        self.process.stdin.flush()

    def send(self, text: str) -> bytes:
        """
        :param text: текст сообщения посылаемого  на сервер
        :return: bytes string with data from server
        """
        self._write(text)
        logging.info("Send request")
        size = self.socket.recv(8)
        size = struct.unpack("@L", size)[0]
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



@dataclass
class MessageParameters:
    name : str
    type : str


def server_run(meta_factory : Generator, values_macros: dict, parameters: MessageParameters, n_workers = None):
    if n_workers is None: n_workers = os.cpu_count()
    input_queue = Queue(maxsize=2*n_workers)
    output_queue = Queue(maxsize=2*n_workers)
    requester = Process(target=generate_request, args=(n_workers, values_macros, input_queue))
    requester.start()
    workers = multythread_server(meta_factory, input_queue, output_queue, n_workers)
    processor = Process(target=process_message, args=(n_workers, output_queue, parameters))
    processor.start()
    return 0



def generate_request(n_workers, values_macros: dict, input_queue: Queue):
    for indx, data in enumerate(request_generator(values_macros, [0.0, 0.0, 0.1])):
        input_queue.put(data)
    for i in range(n_workers):
        input_queue.put("END")
    return 0


def process_message(n_workers, output_queue: Queue, parameters: MessageParameters):
    count = 0
    if parameters.type == "mean":
        run = MeanRun()
    with MeanTable(parameters.name) as mean_table:
        while True:
            message = output_queue.get()
            if message == "END":
                count += 1
                if count == n_workers:
                    break
                continue
            run.ParseFromString(message.data)
            mean_table.append_from_mean_run(run, message.meta)
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
    with Geant4Server(meta) as server:
        for input_data in iter(input_queue.get, "END"):
            text = input_data.data
            meta = input_data.meta
            data = server.send(text)
            output_queue.put(QueueData(meta, data))
    output_queue.put("END")
    return 0

