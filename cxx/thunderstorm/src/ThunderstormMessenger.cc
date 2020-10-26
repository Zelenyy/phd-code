//
// Created by zelenyy on 14.01.2020.
//

#include "ThunderstormMessenger.hh"

G4String ThunderstormMessenger::GetCurrentValue(G4UIcommand *command) {
    if (command == physics) {
        return settings->physics;
    } else if (command == tracking) {
        return settings->tracking;
    } else if (command == minimalEnergy) {
        return to_string(settings->minimal_energy);
    }
    return ServerMessenger::GetCurrentValue(command);
}

void ThunderstormMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
    if (setStackingSettings(command, newValue)) {
        return;
    }
    if (setSteppingSettings(command, newValue)) {
        return;
    }
    if (setTrackingSettings(command, newValue)) {
        return;
    }
    if (command == physics) {
        settings->physics = newValue;
    } else if (command == tracking) {
        settings->tracking = newValue;
    } else if (command == minimalEnergy) {
        settings->minimal_energy = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
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
    } else if (command == aragats_only_muon) {
        settings->aragatsSettings->only_muon = G4UIcmdWithABool::GetNewBoolValue(newValue);
    } else if (command == pie_low) {
        settings->aragatsSettings->low_boundary = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    } else if (command == pie_high) {
        settings->aragatsSettings->high_boundary = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    } else if (command == pie_obs_lvl) {
        settings->aragatsSettings->observed_level = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    } else if (command == parma_particle) {
        settings->parmaSettings->particle->set(newValue);
    } else if (command == parma_position) {
        settings->parmaSettings->position = G4UIcmdWith3VectorAndUnit::GetNew3VectorValue(newValue);
    } else {
        ServerMessenger::SetNewValue(command, newValue);
    }
}


ThunderstormMessenger::ThunderstormMessenger(Settings *pSettings) : ServerMessenger(pSettings), settings(pSettings) {

    thundestorm = new G4UIdirectory(thunderstorm_path.c_str());
    thundestorm->SetGuidance("This is helper");
    initStackingSettings();
    initSteppingSettings();
    initTrackingSettings();

    physics = new G4UIcmdWithAString(physics_path.c_str(), this);
    physics->SetGuidance("Set using physics.");
    physics->SetParameterName("physics", true);

    stackingParticle = new G4UIcmdWithAString(add_particle_stacking_path.c_str(), this);
    stackingParticle->SetGuidance("Add particle in ParticleCylinderStacking.");
    stackingParticle->SetParameterName("particle", false);

    detectorParticle = new G4UIcmdWithAString(add_particle_detector_path.c_str(), this);
    detectorParticle->SetGuidance("Add particle in ParticleDetector.");
    detectorParticle->SetParameterName("particle", false);


    minimalEnergy = new G4UIcmdWithADoubleAndUnit(energy_cut_path.c_str(), this);
    minimalEnergy->SetGuidance("Set minimal energy for particle");
    minimalEnergy->SetParameterName("energy", true, false);
    minimalEnergy->SetDefaultUnit("MeV");


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

    aragats_only_muon = new G4UIcmdWithABool(aragats_only_muon_path.c_str(), this);
    aragats_only_muon->SetParameterName("flag", false);

    aragats_pie = new G4UIdirectory(aragats_pie_path.c_str());
    pie_high = new G4UIcmdWithADoubleAndUnit(pie_high_path.c_str(), this);
    pie_high->SetParameterName("high_boundary", false);
    pie_high->SetDefaultUnit("m");

    pie_low = new G4UIcmdWithADoubleAndUnit(pie_low_path.c_str(), this);
    pie_low->SetParameterName("low_boundary", false);
    pie_low->SetDefaultUnit("m");

    pie_obs_lvl = new G4UIcmdWithADoubleAndUnit(pie_obs_lvl_path.c_str(), this);
    pie_obs_lvl->SetParameterName("observed_level", false);
    pie_obs_lvl->SetDefaultUnit("m");


    parma = new G4UIdirectory(parma_path.c_str());

    parma_particle = new G4UIcmdWithAString(parma_particle_path.c_str(), this);
    parma_particle->SetParameterName("particleName", true);
    parma_particle->SetDefaultValue("mu-");
    G4String candidateList = "mu- mu+";
    parma_particle->SetCandidates(candidateList);

    parma_position = new G4UIcmdWith3VectorAndUnit(parma_position_path.c_str(), this);
    parma_position->SetGuidance("Set starting position of the particle.");
    parma_position->SetParameterName("X", "Y", "Z", true, true);
    parma_position->SetDefaultUnit("cm");
    parma_position->SetUnitCandidates("micron mm cm m km");

}

void ThunderstormMessenger::initStackingSettings() {
    stacking = new G4UIdirectory(stacking_path.c_str());
    enableGamma = new G4UIcmdWithABool(stacking_gamma_path.c_str(), this);
    enableGamma->SetParameterName("flag", false);
    enableMuon = new G4UIcmdWithABool(stacking_muon_path.c_str(), this);
    enableMuon->SetParameterName("flag", false);
    enableElectron = new G4UIcmdWithABool(stacking_electron_path.c_str(), this);
    enableElectron->SetParameterName("flag", false);
    enablePositron = new G4UIcmdWithABool(stacking_positron_path.c_str(), this);
    enablePositron->SetParameterName("flag", false);
    stacking_type = new G4UIcmdWithAString(stacking_type_path.c_str(), this);
    stacking_type->SetGuidance("Set using stacking action.");
    stacking_type->SetParameterName("stacking_type", true);

    saveGamma = new G4UIcmdWithABool(stacking_gamma_save_path.c_str(), this);
    saveGamma->SetParameterName("flag", false);
    saveElectron = new G4UIcmdWithABool(stacking_electron_save_path.c_str(), this);
    saveElectron->SetParameterName("flag", false);

    saveElectronCut = new G4UIcmdWithADoubleAndUnit(stacking_electron_save_cut__path.c_str(), this);
    saveElectronCut->SetParameterName("Minimal energy for save electron", false);
    saveElectronCut->SetDefaultUnit("MeV");

    saveNeutron = new G4UIcmdWithABool(stacking_neutron_save_path.c_str(), this);
    saveNeutron->SetParameterName("flag", false);

}

bool ThunderstormMessenger::setStackingSettings(G4UIcommand *command, G4String newValue) {
    auto stackingSettings = settings->stackingSettings;
    if (command == enableGamma) {
        stackingSettings->enableGamma = G4UIcmdWithABool::GetNewBoolValue(newValue);
    } else if (command == enableMuon) {
        stackingSettings->enableMuon = G4UIcmdWithABool::GetNewBoolValue(newValue);
    } else if (command == enablePositron) {
        stackingSettings->enablePositron = G4UIcmdWithABool::GetNewBoolValue(newValue);
    } else if (command == enableElectron) {
        stackingSettings->enableElectron = G4UIcmdWithABool::GetNewBoolValue(newValue);
    } else if (command == saveGamma) {
        stackingSettings->saveGamma = G4UIcmdWithABool::GetNewBoolValue(newValue);
    } else if (command == saveElectron) {
        stackingSettings->saveElectron = G4UIcmdWithABool::GetNewBoolValue(newValue);
    } else if (command == saveElectronCut) {
        stackingSettings->saveElectronCut = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    } else if (command == saveNeutron) {
        stackingSettings->saveNeutron = G4UIcmdWithABool::GetNewBoolValue(newValue);
    } else if (command == stacking_type) {
        if (newValue == "simple") {
            stackingSettings->type = StackingType::simple;
        }
    } else {
        return false;
    }
    return true;
}


void ThunderstormMessenger::initSteppingSettings() {
    stepping = new G4UIdirectory(stepping_path.c_str());
    stepping_type = new G4UIcmdWithAString(stepping_type_path.c_str(), this);
    stepping_type->SetGuidance("Set using stepping action.");
    stepping_type->SetParameterName("stepping_type", true);
}

bool ThunderstormMessenger::setSteppingSettings(G4UIcommand *command, G4String newValue) {
    auto steppingSettings = settings->steppingSettings;
    if (command == stepping_type) {
        if (newValue == "simple") {
            steppingSettings->type = SteppingType::simple;
        } else if (newValue == "critical_energy") {
            steppingSettings->type = SteppingType::critical_energy;
        }
    } else {
        return false;
    }
    return true;
}

void ThunderstormMessenger::initTrackingSettings() {
    tracking = new G4UIdirectory(tracking_path.c_str());
    trackingSaveGamma = new G4UIcmdWithABool(tracking_gamma_save_path.c_str(), this);
    trackingSaveGamma->SetParameterName("flag", false);
    trackingSaveElectron = new G4UIcmdWithABool(tracking_electron_save_path.c_str(), this);
    trackingSaveElectron->SetParameterName("flag", false);
    trackingSavePositron = new G4UIcmdWithABool(tracking_positron_save_path.c_str(), this);
    trackingSavePositron->SetParameterName("flag", false);
}

bool ThunderstormMessenger::setTrackingSettings(G4UIcommand *command, G4String newValue) {
    auto trackingSettings = settings->trackingSettings;
    if (command == trackingSaveGamma) {
        trackingSettings->saveGamma = G4UIcmdWithABool::GetNewBoolValue(newValue);
    } else if (command == trackingSaveElectron) {
        trackingSettings->saveElectron = G4UIcmdWithABool::GetNewBoolValue(newValue);
    } else if (command == trackingSavePositron) {
        trackingSettings->savePositron = G4UIcmdWithABool::GetNewBoolValue(newValue);
    } else {
        return false;
    }
    return true;
}
