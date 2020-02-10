//
// Created by zelenyy on 09.02.2020.
//

#ifndef PHD_CODE_SOCKETOUTPUT_HH
#define PHD_CODE_SOCKETOUTPUT_HH

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Logger.hh"
#include "vector"

using namespace std;

class ISocketOutput {


public:
    virtual ~ISocketOutput() = default;
    virtual void StopServer() = 0;
};

template<class Data>
class SocketOutput : public ISocketOutput {
private:
    int port;
    int new_socket{};
    int server_fd{};
    vector<Data> dataArray;
    int indxData;
    int maxSizeData;
public:
    explicit SocketOutput(const string &name, int aPort = 8777) {
        this->port = aPort;
        Logger::instance()->print("Open socket " + name + " on address " + to_string(this->port));
        StartServer();

        maxSizeData = 1024;
        indxData = 0;
        dataArray.reserve(maxSizeData);
    };

    void AddDataToSocketStream(const string &data) {
        send(new_socket, data.c_str(), data.length(), 0);
    }

    void addData(Data &data) {
        dataArray[indxData] = data;
        indxData++;
        if (indxData == maxSizeData) {
            writeData();
            indxData = 0;
        }
    }


private:

    void writeData() {
        char *filePointer = (char *) &dataArray.front();
        send(new_socket, filePointer, (sizeof dataArray[0]) * indxData, 0);

    }

    void StartServer() {
        // Server side C/C++ program to demonstrate Socket programming
        struct sockaddr_in address{};
        int opt = 1;
        int addrlen = sizeof(address);
//        char buffer[1024] = {0};

        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                       &opt, sizeof(opt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        // Forcefully attaching socket to the custom port
        if (bind(server_fd, (struct sockaddr *) &address,
                 sizeof(address)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(server_fd, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                                 (socklen_t *) &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }


    }


public:
    void StopServer() override {
        close(server_fd);
    }

    ~SocketOutput() override {
        writeData();
        StopServer();
    };
};

#endif //PHD_CODE_SOCKETOUTPUT_HH
