import socket
import time
import logging
from .satellite_pb2 import Run
from enum import Enum
from string import Template
from typing import List
import subprocess

HOST = '127.0.0.1'  # The server's hostname or IP address
PORT = 8777       # The port used by the server

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
    MEAN = "mean"


class Geant4Server:
    def __init__(self, command: List[str]):
        self.command = command

    def start(self, mode : DetectorMode =DetectorMode.SINGLE):
        logging.info("Start server: {}".format(self.command))
        self.process = subprocess.Popen(self.command,
                                        shell=True,
                                        stdin=subprocess.PIPE,
                                        stdout=subprocess.PIPE)

        text : str = INIT_TEMPLATE.substitute(
            {"mode": mode.value}
        )
        self.process.stdin.write(text.encode())
        self.process.stdin.write(SEPARATOR)
        self.process.stdin.flush()
        return 0

    def send(self, text: str):
        logging.info("Send request")
        self.process.stdin.write(text.encode())
        self.process.stdin.write(SEPARATOR)
        self.process.stdin.flush()
        time.sleep(3)
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
            ultimate_buffer = b''
            while True:
                data = s.recv(1024)
                if not data: break
                logging.debug(data)
                ultimate_buffer += data
        run = Run()
        run.ParseFromString(ultimate_buffer)
        return run

    def stop(self):
        self.process.stdin.write(b"exit\n")
        self.process.stdin.flush()
        self.process.wait()
        logging.info("Stop server")
        return 0