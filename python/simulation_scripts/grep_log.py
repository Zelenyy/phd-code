from phd.utils.path_tools import LogTime
import numpy as np

log = LogTime(".")
paths = np.array(log.paths)
indx = paths.argsort()
count = 1
for t, p in zip(log.time[indx], paths[indx]):
    if (count % 3 == 0):
        print(p,t)
    count+=1