import logging
import numpy as np
import matplotlib.pyplot as plt
from geant4_server import Geant4Server, DetectorMode


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
    logging.root.setLevel(logging.INFO)
    server = Geant4Server(["./build/satellite/geant4-satellite.exe server"])
    server.start(DetectorMode.SINGLE)
    # text = get_request(100)
    # server.send(text)
    for energy in [30, 40, 50, 100]:
        text = get_request(energy)
        run = server.send(text)
        for event in run.event:
            plt.plot(event.deposit)
    server.stop()
    plt.show()
    return 0

if __name__ == '__main__':
    main()