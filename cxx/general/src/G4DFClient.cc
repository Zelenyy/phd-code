//
// Created by zelenyy on 22.11.2019.
//

#include <G4RunManager.hh>
#include <GeneralParticleSource.hh>
#include <G4UImanager.hh>
#include <random>
#include "G4DFClient.hh"
#include "G4DFClientMessenger.hh"


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
    runManager->Initialize();
}

void G4DFClient::read(istream &input) {
    char command[lengthOfCommand];
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    while (input.getline(command, lengthOfCommand)) {
        Logger::instance()->print(
                "Loop number " + to_string(countRead) + " says: \"I get command: " + string(command) + "\"");
        if ((strcmp(command, "\r") == 0) or (strcmp(command, "\r\n") == 0) or (strcmp(command, "\n") == 0) or (strlen(command) == 0))  {
            break;
        }
        UImanager->ApplyCommand(command);
    }
    countRead++;
}

//void G4DFClient::parse_input(int argc_, char **argv_) {
//    this->argc=argc_;
//    this->argv=argv_;
//    for (int i =0; i<argc_; ++i){
//        Logger::instance()->print("See what I get: " + string(argv[i]));
//    }
//    gdml = argv_[1];
//    if (argc_==3){
//        macros = argv_[2];
//    }
//}
