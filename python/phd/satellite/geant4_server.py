import socket
import time
import logging
from .satellite_pb2 import Run
from enum import Enum
from string import Template
from typing import List
import subprocess


INIT_TEMPLATE = Template(
"""/df/project test
/df/gdml satellite.gdml
/satellite/output socket
/satellite/detector ${mode}
"""
)

SEPARATOR = b"\r\n"

class DetectorMode(Enum):
    SINGLE = "single"
    SUM = "sum"


class Geant4Server:
    def __init__(self, command: List[str]):
        self.command = command

    def start(self, mode : DetectorMode =DetectorMode.SINGLE):
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
            {"mode": mode.value}
        )
        self._write(text)
        return 0

    def _write(self, text):
        self.process.stdin.write(text.encode())
        self.process.stdin.write(SEPARATOR)
        self.process.stdin.flush()

    def send(self, text: str, data_host='127.0.0.1', data_port = 8777) -> Run:
        """

        :param text: тескт сообщения посылаемого  на сервер
        :param data_host: адресс хоста на котором сервер будет возвращать даные
        :param data_port: порт через который сервер будет возращать данные
        :return: Run --- десериализованный protbuff
        """
        self._write(text)
        logging.info("Send request")
        time.sleep(3)
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((data_host, data_port))
            ultimate_buffer = b''
            while True:
                data = s.recv(1024)
                if not data: break
                logging.debug(data)
                ultimate_buffer += data
        run = Run()
        run.ParseFromString(ultimate_buffer)
        return run

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        self.stop()
        if exc_val:
             raise

    def stop(self):
        self.process.stdin.write(b"exit\n")
        self.process.stdin.flush()
        self.process.wait()
        logging.info("Stop server")
        return 0