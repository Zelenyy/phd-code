import socket
import time
from enum import Enum
from string import Template
from typing import List

import numpy as np
import matplotlib.pyplot as plt
import subprocess

HOST = '127.0.0.1'  # The server's hostname or IP address
PORT = 8777       # The port used by the server

SOCKET_DTYPE = np.dtype([
    ("deposit", "d", (100)),
])

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
        print("Start server:", self.command)
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


        while True:
            try:
                self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.socket.connect((HOST, PORT))
                break
            except ConnectionRefusedError:
                time.sleep(1)


    def send(self, text: str):
        print("Send request")
        self.process.stdin.write(text.encode())
        self.process.stdin.write(SEPARATOR)
        self.process.stdin.flush()

        ultimate_buffer = b''
        while True:
            data = self.socket.recv(800)
            print(data)
            if not data: break
            ultimate_buffer += data
        data = np.frombuffer(ultimate_buffer, dtype=SOCKET_DTYPE)
        return data

    def stop(self):
        self.process.stdin.write(b"exit")
        self.process.wait()
        print("Stop server")

def get_request(energy=100, theta=0, number = 1):
    """

    :param energy: Energy in MeV
    :param theta: Theta angle in degree
    :return:
    """
    particle = "/gps/particle proton\n" # Set particle type
    energy = "/gps/ene/mono {} MeV\n".format(energy)

    theta = np.deg2rad(theta)
    direction = "/gps/direction {} 0 {}\n".format(np.sin(theta), -np.cos(theta))

    radius = 0.15
    position = "/gps/position {} 0. {} m\n".format(radius*np.sin(theta), radius*np.cos(theta))

    number = "/run/beamOn {}\n".format(number)
    return particle+energy+direction+position+number

def main():
    server = Geant4Server(["./build/satellite/geant4-satellite.exe server"])
    server.start(DetectorMode.SINGLE)

    for energy in [30, 40, 50, 100]:
        text = get_request(energy)
        data = server.send(text)
        plt.plot(data[0])
    server.stop()
    plt.show()
    return 0

if __name__ == '__main__':
    main()