#include <SatelliteMessenger.hh>
#include <G4UImanager.hh>
#include <G4UIExecutive.hh>
#include <QGSP_BERT.hh>
#include <G4Server.hh>
#include "Settings.hh"
#include "ServerSettings.hh"
//Mandatory user option
#include "ActionInitialization.hh"
//Factory
#include "SensitiveDetectorFactory.hh"
#include "IFieldFactory.hh"



int main(int argc, char **argv) {
    auto logger = Logger::instance();
    logger->print("Number of arguments: " + to_string(argc));
    for (int i = 0; i < argc; ++i) {
        logger->print(string("Argument number ") + to_string(i) + ": " + argv[i]);
    }

    auto settings = new Settings();
    settings->argc = argc;
    settings->argv = argv;
    auto messender = new SatelliteMessenger(settings);
    auto g4Server = new G4Server(messender, settings);

    // Select input
    istream *setup_in = &std::cin;
    istream *mainloop_in = &std::cin;

    vector<fstream*> open_files;
    if (argc > 1) {
        if (strcmp(argv[1], "vis") == 0){

        }
        else{
            string filename = argv[1];
            if (strcmp(argv[1], "test") == 0) {
                filename = "init.mac";
            }
            fstream *fin = new fstream;
            fin->open(filename);
            open_files.push_back(fin);
            setup_in = fin;
            mainloop_in = fin;
        }

    }

    // Run server
    g4Server->setup(*setup_in);
    if (g4Server->massWorld == nullptr) {
        cout<<"Not geometry"<<endl;
        return 0;
    } else {
        g4Server->massWorld->setDetectorFactory(new SensitiveDetectorFactory(settings));
        g4Server->massWorld->setFieldFactory(new IFieldFactory);
    }

    auto physList = new QGSP_BERT(-1);
    g4Server->runManager->SetUserInitialization(physList);
    auto actionInit = new ActionInitialization(settings);
    g4Server->runManager->SetUserInitialization(actionInit);

    g4Server->mainloop(*mainloop_in);
    g4Server->stop();
    for ( auto &&fin : open_files){
        fin->close();
    }
    return 0;
}

