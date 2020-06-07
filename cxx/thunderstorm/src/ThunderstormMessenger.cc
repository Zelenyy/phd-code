//
// Created by zelenyy on 14.01.2020.
//

#include "ThunderstormMessenger.hh"

G4String ThunderstormMessenger::GetCurrentValue(G4UIcommand *command) {
    if (command == physics) {
        return settings->physics;
    } else if (command == stacking) {
        return settings->stacking;
    } else if (command == stepping) {
        return settings->stepping;
    } else if (command == tracking) {
        return settings->tracking;
    } else if (command == energyCut) {
        return to_string(settings->born_cut);
    }
    return ServerMessenger::GetCurrentValue(command);
}

void ThunderstormMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
    if (command == physics) {
        settings->physics = newValue;
    } else if (command == stacking) {
        settings->stacking = newValue;
    } else if (command == stepping) {
        settings->stepping = newValue;
    } else if (command == tracking) {
        settings->tracking = newValue;
    } else if (command == energyCut) {
        settings->born_cut = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    } else if (command == stackingParticle) {
        settings->particle_cylinder_stacking.push_back(newValue);
    } else if (command == detectorParticle) {
        settings->particle_detector.push_back(newValue);
    } else if (command == geo_height) {
        settings->geometrySettings->height = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    } else if (command == field_z) {
        settings->geometrySettings->field_z = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    } else if (command == aragats_geo_type) {
        if (newValue == "uniform") {
            settings->aragatsSettings->aragatsGeoType = AragatsGeoType::uniform;
        } else if (newValue == "pie") {
            settings->aragatsSettings->aragatsGeoType = AragatsGeoType::pie;
        }
    } else {
        ServerMessenger::SetNewValue(command, newValue);
    }
}

ThunderstormMessenger::ThunderstormMessenger(Settings *pSettings) : ServerMessenger(pSettings), settings(pSettings) {

    thundestorm = new G4UIdirectory(thunderstorm_path.c_str());
    thundestorm->SetGuidance("This is helper");

    physics = new G4UIcmdWithAString(physics_path.c_str(), this);
    physics->SetGuidance("Set using physics.");
    physics->SetParameterName("physics", true);

    stacking = new G4UIcmdWithAString(stacking_path.c_str(), this);
    stacking->SetGuidance("Set using stacking action.");
    stacking->SetParameterName("stacking", true);

    stepping = new G4UIcmdWithAString(stepping_path.c_str(), this);
    stepping->SetGuidance("Set using stepping action.");
    stepping->SetParameterName("stepping", true);

    tracking = new G4UIcmdWithAString(tracking_path.c_str(), this);
    tracking->SetGuidance("Set using tracking action.");
    tracking->SetParameterName("tracking", true);

    stackingParticle = new G4UIcmdWithAString(add_particle_stacking_path.c_str(), this);
    stackingParticle->SetGuidance("Add particle in ParticleCylinderStacking.");
    stackingParticle->SetParameterName("particle", false);

    detectorParticle = new G4UIcmdWithAString(add_particle_detector_path.c_str(), this);
    detectorParticle->SetGuidance("Add particle in ParticleDetector.");
    detectorParticle->SetParameterName("particle", false);

    auto cutDirectory = new G4UIdirectory(cut_path.c_str());
    energyCut = new G4UIcmdWithADoubleAndUnit(energy_cut_path.c_str(), this);
    energyCut->SetGuidance("Set minimal energy for particle");
    energyCut->SetParameterName("energy", true, false);
    energyCut->SetDefaultUnit("MeV");


    geo_height = new G4UIcmdWithADoubleAndUnit(geo_height_path.c_str(), this);
    geo_height->SetGuidance("Set height of cloud");
    geo_height->SetParameterName("height", false);
    geo_height->SetDefaultUnit("m");

    field_z = new G4UIcmdWithADoubleAndUnit(field_z_path.c_str(), this);
    field_z->SetGuidance("Set uniform Z-field in cloud");
    field_z->SetParameterName("field", false);
    field_z->SetDefaultUnit("kV/m");

    aragats = new G4UIdirectory(aragats_path.c_str());
    aragats->SetGuidance("This is helper");

    aragats_geo_type = new G4UIcmdWithAString(aragats_geo_type_path.c_str(), this);
    aragats_geo_type->SetParameterName("type", false);
    aragats_geo_type->SetCandidates("uniform pie");

}
