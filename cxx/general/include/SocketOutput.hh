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


#define SO_REUSEPORT 15

using namespace std;


class SocketOutput{
private:
    int port;
    int new_socket{};
    int server_fd{};
public:
    explicit SocketOutput(const string &name, int aPort = 8777) {
        this->port = aPort;
        Logger::instance()->print("Open socket " + name + " on address " + to_string(this->port));
        openSocket();
    };

    int getID(){ return new_socket;};

    void write(unsigned long int data){
        char* buff;
        size_t buffsize = sizeof(data);
//        for (size_t i = buffsize; i!=0; --i){
//            buff[i] = (int) ((data >> 8*(buffsize-i)) & 0XFF);
//        }
        buff = (char*)(&data);
        ssize_t res =send(new_socket, buff, buffsize, 0);
    }

    void write(string data){
        auto buff = data.c_str();
        size_t buffsize = data.length();
        while(buffsize > 0) {
            ssize_t res =send(new_socket, buff, buffsize, 0);
            if (res<=0){
                if(errno == EINTR)
                    continue;
                throw std::runtime_error("write() failed");
            }
            buff += res;
            buffsize -=res;
        }
    }


private:

    void openSocket() {
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
    };
public:
    void closeSocket(){
        shutdown(server_fd, SHUT_WR);
        close(server_fd);
        close(new_socket);
    }

public:
    ~SocketOutput() {
    };
};

#endif //PHD_CODE_SOCKETOUTPUT_HH
