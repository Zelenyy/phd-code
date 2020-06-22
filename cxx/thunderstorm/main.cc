#include <G4DFClient.hh>
#include <ThunderstormMessenger.hh>

#include <ThunderstormConstruction.hh>
#include "Settings.hh"
//Mandatory user option
#include "ActionInitialization.hh"
#include "PhysicsList.hh"
//Factory
#include "SensitiveDetectorFactory.hh"
#include "FieldFactory.hh"


#include "G4Server.hh"

int main(int argc, char **argv) {

    GeometryType type = GeometryType::gdml;
    type = GeometryType ::gdml;


    auto logger = Logger::instance();
    logger->print("Number of arguments: " + to_string(argc));
    for (int i = 0; i < argc; ++i) {
        logger->print(string("Argument number ") + to_string(i) + ": " + argv[i]);
    }

    // Create server and settings
    auto settings = new Settings();
    settings->argc = argc;
    settings->argv = argv;
    auto messender = new ThunderstormMessenger(settings);
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
        ThunderstormConstruction *construction;
        if (settings->geoSubType == ThunderstormSubType::aragats){
            construction = new AragatsConstruction(settings);
        }

//        g4Server->massWorld = thunderstormDC;
        g4Server->runManager->SetUserInitialization(construction);
    } else {
        g4Server->massWorld->setDetectorFactory(new SensitiveDetectorFactory(settings));
        g4Server->massWorld->setFieldFactory(new IFieldFactory);
    }

    auto physList = new PhysicsList(settings->physics);
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

//
//    if (argc > 1) {

//        else if (strcmp(argv[1], "vis") == 0){

//            G4UIExecutive* ui =  new G4UIExecutive(1, argv);
//            G4VisManager* visManager = new G4VisExecutive;
//            visManager->Initialize();
//            G4UImanager* UImanager = G4UImanager::GetUIpointer();
//            UImanager->ApplyCommand("/control/execute init_vis.mac");
//            ui->SessionStart();
//            delete ui;
//            dfClient->stop();
//            fin.close();
//        }




