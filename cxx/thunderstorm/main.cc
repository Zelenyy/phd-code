#include <G4DFClient.hh>
#include <ThunderstormMessenger.hh>
#include "Settings.hh"
//Mandatory user option
#include "ActionInitialization.hh"
#include "PhysicsList.hh"
//Factory
#include "SensitiveDetectorFactory.hh"
#include "FieldFactory.hh"


int main(int argc, char **argv) {
    auto logger = Logger::instance();
    logger->print("Number of arguments: " + to_string(argc));
    for (int i = 0; i < argc; ++i) {
        logger->print(string("Argument number ") + to_string(i) + ": " + argv[i]);
    }

    auto settings = new Settings();
    auto messender = ThunderstormMessenger(settings);

    if (argc > 1) {
        if (strcmp(argv[1], "test") == 0) {
            fstream fin;
            fin.open("init.mac");
            G4DFClient *dfClient = G4DFClient::instance();
            dfClient->read(fin);
            dfClient->setup(new PhysicsList(settings->physics), new ActionInitialization(settings));
            dfClient->initialize();
            dfClient->massWorld->setDetectorFactory(new SensitiveDetectorFactory);
            dfClient->read(fin);
            dfClient->stop();
            fin.close();
        }
    }
    else{
        G4DFClient *dfClient = G4DFClient::instance();
        dfClient->read(std::cin);
        dfClient->setup(new PhysicsList(settings->physics), new ActionInitialization(settings));
        dfClient->initialize();
        dfClient->massWorld->setDetectorFactory(new SensitiveDetectorFactory);
        dfClient->read(std::cin);
        dfClient->stop();
    }
    return 0;
}



