import logging
import os

import numpy as np

from phd.satellite.satellite_pb2 import MeanRun
from tables import File, Group, IsDescription, Float32Col, Int32Col
from .satellite_pb2 import Run


class DepositDescription(IsDescription):
    event = Float32Col(100)

class MeanRunDescription(IsDescription):
    mean = Float32Col(100)
    variance = Float32Col(100)
    number = Int32Col()

logger = logging.getLogger(__name__)
logger.addHandler(
    logging.FileHandler("satellite_convertor.log")
)


def convert_satellite_proto(path, h5file: File, group: Group, settings):
    run = Run()
    logger.info("Open file: {}".format(path))
    with open(path, "rb") as fin:
        run.ParseFromString(fin.read())

    n = len(run.event[0].deposit)
    name = os.path.split(path)[1].split(".")[0]

    table = h5file.create_table(group,
                                name=name,
                                description=np.dtype([("event", "d",(n,))]),
                                title="Deposit in detector cell", **settings)
    deposit = table.row
    for event in run.event:
        deposit["event"] = event.deposit
        deposit.append()
    table.flush()
    return 0

def convert_mean_run_proto(path, h5file: File, group: Group, settings):
    run = MeanRun()
    logger.info("Open file: {}".format(path))
    with open(path, "rb") as fin:
        run.ParseFromString(fin.read())

    table = h5file.create_table(group,
                                name="mean_deposit",
                                description=DepositDescription,
                                title="Mean deposit in detector cell", **settings)
    row = table.row
    row["mean"] = run.mean
    row["variance"] = run.variance
    row["number"] = run.number
    row.append()
    table.flush()