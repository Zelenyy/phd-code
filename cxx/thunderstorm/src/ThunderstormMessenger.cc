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
    else if (command == stepping){
        return settings->stepping;
    }
    else if (command == tracking){
        return settings->tracking;
    }
    else if (command == energyCut){
        return to_string(settings->born_cut);
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
    else if (command == stepping){
        settings->stepping = newValue;
    }
    else if (command == tracking){
        settings->tracking=newValue;
    }
    else if (command == energyCut){
        settings->born_cut = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    } else if (command == stackingParticle){
        settings->particle_cylinder_stacking.push_back(newValue);
    } else if (command == detectorParticle){
        settings->particle_detector.push_back(newValue);
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

   stepping = new G4UIcmdWithAString(stepping_path.c_str(), this);
    stepping ->SetGuidance("Set using stepping action.");
    stepping ->SetParameterName("stepping", true);

    tracking = new G4UIcmdWithAString(tracking_path.c_str(), this);
    tracking ->SetGuidance("Set using tracking action.");
    tracking ->SetParameterName("tracking", true);

    stackingParticle = new G4UIcmdWithAString(add_particle_stacking_path.c_str(), this);
    stackingParticle ->SetGuidance("Add particle in ParticleCylinderStacking.");
    stackingParticle ->SetParameterName("particle", false);

    detectorParticle = new G4UIcmdWithAString(add_particle_detector_path.c_str(), this);
    detectorParticle ->SetGuidance("Add particle in ParticleDetector.");
    detectorParticle ->SetParameterName("particle", false);

    auto cutDirectory = new G4UIdirectory(cut_path.c_str());
    energyCut = new G4UIcmdWithADoubleAndUnit(energy_cut_path.c_str(), this);
    energyCut->SetGuidance("Set minimal energy for particle");
    energyCut->SetParameterName("energy", true, false);
    energyCut->SetDefaultUnit("MeV");

}
