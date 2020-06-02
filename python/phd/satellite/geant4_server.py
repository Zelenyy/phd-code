import os
import socket
import struct
import time
import logging
from dataclasses import dataclass
from enum import Enum
from string import Template
from typing import List, Callable
import subprocess


INIT_TEMPLATE = Template(
"""/df/project test
/df/gdml ${gdml}
/satellite/output socket
/satellite/port ${port}
/satellite/detector ${mode}
"""
)

SEPARATOR = b"\r\n"

class DetectorMode(Enum):
    SINGLE = "single"
    SUM = "sum"

class Geant4Server:
    def __init__(self, command: List[str], meta: dict):
        self.command = command
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
                print("sleep")
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


from multiprocessing import Process, Pipe, Queue

# def multythread_server(commnad, meta_factory : Generator, request_factory: Generator, processor: Callable, n_workers = None):
#     if n_workers is None: n_workers = os.cpu_count()
#
#     workers = []
#     pipes = []
#     for i in range(n_workers):
#         parent, child = Pipe()
#         meta = next(meta_factory)
#         worker = Process(target=start_server_in_multythread, args=(commnad, meta, child))
#         worker.start()
#         workers.append(worker)
#         pipes.append(parent)
#
#     count = 0
#
#     while True:
#         try:
#             request = next(request_factory)
#             i = count % n_workers
#             pipe = pipes[i]
#             pipe.send(request)
#             count+=1
#             if (count // n_workers) % 3 == 0:
#                 for pipe in pipes:
#                     while True:
#                         result = pipe.recv()
#                         if s == "":
#                             break
#
#         except StopIteration:
#             for pipe, worker in zip(pipes, workers):
#                 pipe.send("stop")
#             break
#     for worker in workers:
#         worker.join()

# def multythread_server(commnad, meta_factory : Generator, input_queue: Queue, output_queue: Queue, n_workers = None):
#     workers = []
#     for i in range(n_workers):
#         meta = next(meta_factory)
#         worker = Process(target=start_server_in_thread, args=(commnad, meta, input_queue, output_queue))
#         worker.start()
#         workers.append(worker)
#     return workers

def start_server_in_thread(command, meta, input_queue: Queue, output_queue: Queue):
    with Geant4Server(command, meta) as server:
        while True:
            text = input_queue.get()
            data = server.send(text)
            output_queue.put(data)
            input_queue.task_done()
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
