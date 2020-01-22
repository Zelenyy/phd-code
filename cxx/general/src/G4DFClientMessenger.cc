//
// Created by zelenyy on 22.11.2019.
//

#include <G4UIcommand.hh>
#include <G4UImessenger.hh>
#include "G4DFClientMessenger.hh"
#include "G4DFClient.hh"

G4DFClientMessenger::~G4DFClientMessenger() {
    delete project;
}

void G4DFClientMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
    if (command == project) {
        dfClient->setProject(newValue);
    } else if (command == gdml) {
        dfClient->setGDML(newValue);
    } else if (command == seed) {
        dfClient->setSeed(G4UIcmdWithAnInteger::GetNewIntValue(newValue));
    }
}

G4String G4DFClientMessenger::GetCurrentValue(G4UIcommand *command) {
    string result;
    if (command == project) {
        result = "project";
    } else if (command == gdml) {
        result = "default.gdml";
    } else {
        result = G4UImessenger::GetCurrentValue(command);
    }
    return result;
}

G4DFClientMessenger::G4DFClientMessenger(G4DFClient *client) : dfClient(client) {

    directory = new G4UIdirectory(df_directory.c_str());
    directory->SetGuidance("This is helper");

    project = new G4UIcmdWithAString(project_path.c_str(), this);
    project->SetGuidance("Set project name.");
    project->SetParameterName("name", false);

    gdml = new G4UIcmdWithAString(gdml_path.c_str(), this);
    gdml->SetGuidance("Set gdml file with geometry.");
    gdml->SetParameterName("filename", false);

    seed = new G4UIcmdWithAnInteger(seed_path.c_str(), this);
    seed->SetGuidance("Set seed for random engine");
    seed->SetParameterName("seed", true);
    seed->SetDefaultValue(0);
}
