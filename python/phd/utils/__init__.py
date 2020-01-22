from collections import OrderedDict


def summarize(keys, values, updater, orddict : OrderedDict) -> OrderedDict:
    for k,v in zip(keys, values):
        if k in orddict.keys():
            updater(k,v, orddict)
        else:
            orddict[k] = v
    return orddict