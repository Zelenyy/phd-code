#include <G4DFClient.hh>
#include <SatelliteMessenger.hh>
#include <G4VisManager.hh>
#include <G4VisExecutive.hh>
#include <G4UImanager.hh>
#include <G4UIExecutive.hh>
#include <QGSP_BERT.hh>
#include "Settings.hh"
//Mandatory user option
#include "ActionInitialization.hh"
//Factory
#include "SensitiveDetectorFactory.hh"
#include "IFieldFactory.hh"
#include "DataStorage.hh"

void setupSimulation(G4DFClient *dfClient, Settings *settings) {
    dfClient->setup(new QGSP_BERT(-1), new ActionInitialization(settings));
    dfClient->massWorld->setDetectorFactory(new SensitiveDetectorFactory(settings));
    dfClient->massWorld->setFieldFactory(new IFieldFactory);
    dfClient->initialize();
}

void startFromFile(G4DFClient *dfClient, const string &filename, Settings *settings) {
    fstream fin;
    fin.open(filename);
    dfClient->read(fin);
    setupSimulation(dfClient, settings);
    dfClient->read(fin);
    fin.close();
}

int main(int argc, char **argv) {
    auto logger = Logger::instance();
    logger->print("Number of arguments: " + to_string(argc));
    for (int i = 0; i < argc; ++i) {
        logger->print(string("Argument number ") + to_string(i) + ": " + argv[i]);
    }

    auto settings = new Settings();
    auto messender = SatelliteMessenger(settings);
    G4DFClient *dfClient = G4DFClient::instance();
    if (argc > 1) {
        if (strcmp(argv[1], "test") == 0) {
            startFromFile(dfClient, "init.mac", settings);
        } else if (strcmp(argv[1], "vis") == 0) {
            startFromFile(dfClient, argv[2], settings);
            G4UIExecutive *ui = new G4UIExecutive(1, argv);
            G4VisManager *visManager = new G4VisExecutive;
            visManager->Initialize();
            G4UImanager *UImanager = G4UImanager::GetUIpointer();
            UImanager->ApplyCommand("/control/execute init_vis.mac");
            ui->SessionStart();
            delete ui;
        } else if (strcmp(argv[1], "server") == 0) {
            logger->print("Start cin server");
            dfClient->read(std::cin);
            setupSimulation(dfClient, settings);
            while (true) {
                logger->print("Start next run");
                int code = dfClient->read(std::cin);
                if (code == -1) {
                    break;
                }
            }

        } else {
            startFromFile(dfClient, argv[1], settings);
        }
    } else {
        dfClient->read(std::cin);
        setupSimulation(dfClient, settings);
        dfClient->read(std::cin);
    }
    dfClient->stop();
    return 0;
}



