//
// Created by zelenyy on 05.06.2020.
//

#ifndef PHD_CODE_G4SERVER_HH
#define PHD_CODE_G4SERVER_HH

#include "G4RunManager.hh"
#include "GeneralParticleSource.hh"
#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>
#include "G4UIcmdWithABool.hh"
#include <random>
#include <G4UImanager.hh>
#include "DetectorConstruction.hh"
#include <G4VisManager.hh>
#include <G4VisExecutive.hh>
#include <G4UImanager.hh>
#include <G4UIExecutive.hh>

enum class ReadStatus {
    continue_,
    exit
};

enum class GeometryType {
    gdml,
    custom
};

enum class PrimaryGeneratorType {
    gps,
    custom
};



class ServerSettings {
public:
    GeometryType geometry = GeometryType::gdml;
    std::string gdml = "default.gdml";
    PrimaryGeneratorType generator = PrimaryGeneratorType::gps;
    long seed = -1;
    int numberOfAdditionalWaitingStacks = 4;
    int lengthOfCommand = 1000;
    bool visualization = false;
    int argc = 0;
    char** argv;
};

class ServerMessenger : public G4UImessenger {
public:
    ServerMessenger(ServerSettings *serverSettings) : serverSettings(serverSettings) {

        root = new G4UIdirectory(root_path.c_str());
        root->SetGuidance("This is helper");

        gdml = new G4UIcmdWithAString(gdml_path.c_str(), this);
        gdml->SetGuidance("Set gdml file with geometry.");
        gdml->SetParameterName("filename", false);

        seed = new G4UIcmdWithAnInteger(seed_path.c_str(), this);
        seed->SetGuidance("Set seed for random engine");
        seed->SetParameterName("seed", true);
        seed->SetDefaultValue(0);


        geometry = new G4UIdirectory(geo_path.c_str());
        geo_type = new G4UIcmdWithAString(geo_type_path.c_str(), this);
        geo_type->SetParameterName("type", false);
        geo_type->SetCandidates("gdml custom");

        generator = new G4UIdirectory(gen_path.c_str());
        gen_type = new G4UIcmdWithAString(gen_type_path.c_str(), this);
        gen_type->SetParameterName("type", false);
        gen_type->SetCandidates("gps custom");

        visualization = new G4UIcmdWithABool(visualization_path.c_str(), this);
        visualization->SetParameterName("flag", false);

    };

    G4String GetCurrentValue(G4UIcommand *command) override {
        // TODO()
        std::string result;
        if (command == gdml) {
            result = serverSettings->gdml;
        } else {
            result = G4UImessenger::GetCurrentValue(command);
        }
        return result;
    }

    void SetNewValue(G4UIcommand *command, G4String newValue) override {
        if (command == gdml) {
            serverSettings->gdml = newValue;
        } else if (command == seed) {
            serverSettings->seed = G4UIcmdWithAnInteger::GetNewIntValue(newValue);
        } else if (command == geo_type) {
            if (newValue == "gdml"){
                serverSettings->geometry = GeometryType::gdml;
            }
            else if (newValue == "custom"){
                serverSettings->geometry = GeometryType::custom;
            }
        } else if (command == gen_type){
            if (newValue == "gps"){
                serverSettings->generator = PrimaryGeneratorType::gps;
            }
            else if (newValue == "custom"){
                serverSettings->generator = PrimaryGeneratorType::custom;
            }
        }
        else if (command == visualization){
            serverSettings->visualization = G4UIcmdWithABool::GetNewBoolValue(newValue);
        }
        else {
            G4UImessenger::SetNewValue(command, newValue);
        }
    }

protected:
    G4UIdirectory *root;
    std::string root_path = "/npm/";

    G4UIcmdWithAnInteger *seed;
    std::string seed_path = root_path + "seed";
    G4UIdirectory *geometry;
    std::string geo_path = root_path + "geometry/";

    G4UIcmdWithAString *geo_type;
    std::string geo_type_path = geo_path + "type";

    G4UIcmdWithAString *gdml;
    std::string gdml_path = geo_path + "gdml";

    G4UIcmdWithABool * visualization;
    std::string visualization_path = root_path + "visualization";

    G4UIdirectory *generator;
    std::string gen_path = root_path + "generator/";

    G4UIcmdWithAString *gen_type;
    std::string gen_type_path = gen_path + "type";




protected:
    ServerSettings *serverSettings;
};

class G4Server {
public:
    G4RunManager *runManager;
    DetectorConstruction *massWorld = nullptr;
    ServerSettings *serverSettings;
private:
    ServerMessenger *serverMessenger;
    Logger *logger;

public:
    explicit G4Server(ServerMessenger *serverMessenger, ServerSettings *serverSettings)
    : serverSettings(serverSettings), serverMessenger(serverMessenger){
        logger = Logger::instance();
    }

    void setup(istream &input) {
        logger->print("Start setup server");
        read(input);

        long seed = serverSettings->seed;
        if (seed < 0) {
            std::random_device rd;
            uniform_int_distribution<long> uid(0, LONG_MAX);
            seed = uid(rd);
        }
        HepRandom::setTheSeed(seed);

        runManager = new G4RunManager;
        if (serverSettings->geometry == GeometryType::gdml) {
            massWorld = new DetectorConstruction(serverSettings->gdml);
            runManager->SetUserInitialization(massWorld);
        }


        runManager->SetNumberOfAdditionalWaitingStacks(serverSettings->numberOfAdditionalWaitingStacks);


    }

    void mainloop(istream &input) {
        if (serverSettings->generator == PrimaryGeneratorType::gps) {
            auto *generalParticleSource = new GeneralParticleSource();
            runManager->SetUserAction(generalParticleSource);
        }
        runManager->Initialize();
        if (serverSettings->visualization){
            read(input);
            G4UIExecutive* ui =  new G4UIExecutive(serverSettings->argc, serverSettings->argv);
            G4VisManager* visManager = new G4VisExecutive;
            visManager->Initialize();
            G4UImanager* UImanager = G4UImanager::GetUIpointer();
            UImanager->ApplyCommand("/control/execute init_vis.mac");
            ui->SessionStart();
            delete ui;
        } else{
            ReadStatus exit_code;
            do {
                logger->print("Start next run");
                exit_code = read(input);
            } while (exit_code != ReadStatus::exit);
        }


    }

    void stop() {
        delete runManager;
    }

private:

    int countRead = 1;

    ReadStatus read(istream &input) {
        int lengthOfCommand = serverSettings->lengthOfCommand;
        ReadStatus exit_code = ReadStatus::continue_;
        char command[lengthOfCommand];
        G4UImanager *UImanager = G4UImanager::GetUIpointer();
        while (input.getline(command, lengthOfCommand)) {
            logger->print(
                    "Loop number " + to_string(countRead) + " says: \"I get command: " + string(command) + "\"");
            if ((strcmp(command, "\r") == 0) or (strcmp(command, "\r\n") == 0) or (strcmp(command, "\n") == 0) or
                (strlen(command) == 0)) {
                break;
            }
            if (strcmp(command, "separator") == 0){
                break;
            }
            if (strcmp(command, "exit") == 0) {
                exit_code = ReadStatus::exit;
                break;
            }
            UImanager->ApplyCommand(string(command));
        }
        countRead++;
        return exit_code;
    }

};

#endif //PHD_CODE_G4SERVER_HH
