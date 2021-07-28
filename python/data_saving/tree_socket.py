
import socket
import numpy as np
import matplotlib as plt

HOST = '127.0.0.1'  # The server's hostname or IP address
PORT = 8777
# TREE_SOCKET_DTYPE = np.dtype([
#     ("id", "i"),
#     ("parent_id", "i"),
#     ("particle", "i"),
#     ("zero", "i"),
#     ("energy", "d"),
#     ("theta", "d"),
#     ("radius", "d"),
#     ("z", "d"),
# ])

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        ultimate_buffer = b''
        while True:
            data = s.recv(1024)
            if not data : break
            ultimate_buffer += data
        print(ultimate_buffer)
        # print(repr(ultimate_buffer))
        # print(len(ultimate_buffer), TREE_SOCKET_DTYPE.itemsize)
        # data = np.frombuffer(ultimate_buffer, dtype=TREE_SOCKET_DTYPE)
        # np.save("tree_socket2", data)

if __name__ == '__main__':
    main()