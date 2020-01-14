//
// Created by zelenyy on 14.01.2020.
//

#include "ThunderstormMessenger.hh"

G4String ThunderstormMessenger::GetCurrentValue(G4UIcommand *command) {
    if (command == physics){
        return settings->physics;
    }
    else if( command == stacking){
        return settings->stacking;
    }
    return G4UImessenger::GetCurrentValue(command);
}

void ThunderstormMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
    if (command==physics){
        settings->physics = newValue;
    }
    else if (command==stacking){
        settings->stacking = newValue;
    }
    else{
        G4UImessenger::SetNewValue(command, newValue);
    }
}

ThunderstormMessenger::ThunderstormMessenger(Settings* pSettings) : settings(pSettings) {
    directory = new G4UIdirectory(thunderstorm_directory.c_str());
    directory->SetGuidance("This is helper");
    physics = new G4UIcmdWithAString(physics_path.c_str(), this);
    physics ->SetGuidance("Set using physics.");
    physics ->SetParameterName("physics", true);

    stacking = new G4UIcmdWithAString(stacking_path.c_str(), this);
    stacking ->SetGuidance("Set using stacking action.");
    stacking ->SetParameterName("stacking", true);
}
