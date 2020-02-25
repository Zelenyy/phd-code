//
// Created by zelenyy on 22.11.2019.
//

#include <G4RunManager.hh>
#include <GeneralParticleSource.hh>
#include <G4UImanager.hh>
#include <random>
#include "G4DFClient.hh"
#include "G4DFClientMessenger.hh"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

G4DFClient::G4DFClient() {
    messenger = new G4DFClientMessenger(this);
}


void G4DFClient::setup(G4VUserPhysicsList *physList, G4VUserActionInitialization *actions) {
    if (seed < 0){
        random_device rd;
        uniform_int_distribution<long> uid(0, LONG_MAX);
        seed = uid(rd);
    }
    HepRandom::setTheSeed(seed);
    Logger::instance()->print("Set the seed: " + to_string(HepRandom::getTheSeed()));
    runManager = new G4RunManager;
    massWorld = new DetectorConstruction(gdml);
    runManager->SetUserInitialization(massWorld);
    runManager->SetUserInitialization(physList);
    auto *generalParticleSource = new GeneralParticleSource();
    runManager->SetUserAction(generalParticleSource);
    if (actions != nullptr) {
        runManager->SetUserInitialization(actions);
    }

}

void G4DFClient::initialize() {
    runManager->SetNumberOfAdditionalWaitingStacks(4);
    runManager->Initialize();
}

int G4DFClient::read(istream &input) {
    int exit_code = 0;
    char command[lengthOfCommand];
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    while (input.getline(command, lengthOfCommand)) {
        Logger::instance()->print(
                "Loop number " + to_string(countRead) + " says: \"I get command: " + string(command) + "\"");
        if ((strcmp(command, "\r") == 0) or (strcmp(command, "\r\n") == 0) or (strcmp(command, "\n") == 0) or (strlen(command) == 0))  {
            break;
        }
        if (strcmp(command, "exit") == 0){
            exit_code = -1;
            break;
        }
        UImanager->ApplyCommand(string(command));
    }
    countRead++;
    return exit_code;
}

void G4DFClient::startSocketServer() {
    //        // Server side C/C++ program to demonstrate Socket programming
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
        address.sin_family = AF_LOCAL;
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

