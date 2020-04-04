import logging

from tables import File, Group, IsDescription, Float32Col
from .satellite_pb2 import Run


class DepositDescription(IsDescription):
    event = Float32Col(100)


logger = logging.getLogger(__name__)
logger.addHandler(
    logging.FileHandler("satellite_convertor.log")
)


def convert_satellite_proto(path, h5file: File, group: Group, settings):
    run = Run()
    logger.info("Open file: {}".format(path))
    with open(path, "rb") as fin:
        run.ParseFromString(fin.read())

    table = h5file.create_table(group,
                                name="deposit",
                                description=DepositDescription,
                                title="Deposit in detector cell", **settings)
    deposit = table.row
    for event in run.event:
        deposit["event"] = event.deposit
        deposit.append()
    table.flush()
    return 0
