import numpy as np


def find_chain(data, parent_final):
    result = []
    parent_indx = np.where(data["id"] == parent_final)[0][0]
    parent_id = data[parent_indx]["parent_id"]
    result.append(parent_final)
    if parent_id == 0:
        return [1]
    result += find_chain(data, parent_id)
    return result

def get_chains(data, data_new_seed):
    chains = []
    for item in data_new_seed:
        chains.append(find_chain(data, item["parent_id"]))
    return chains