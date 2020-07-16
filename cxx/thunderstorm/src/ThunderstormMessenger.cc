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
    } else if (command == aragats_only_muon) {
        settings->aragatsSettings->only_muon = G4UIcmdWithABool::GetNewBoolValue(newValue);
    } else if (command == pie_low) {
        settings->aragatsSettings->low_boundary = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    } else if (command == pie_high) {
        settings->aragatsSettings->high_boundary = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    } else if (command == pie_obs_lvl) {
        settings->aragatsSettings->observed_level = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValue);
    } else if (command == parma_particle){
//        particleTable = G4ParticleTable::GetParticleTable();
//        G4ParticleDefinition* pd = particleTable->FindParticle(newValue);
        settings->parmaSettings->particle->set(newValue);
    } else if (command == parma_position){
        settings->parmaSettings->position = G4UIcmdWith3VectorAndUnit::GetNew3VectorValue(newValue);
    }
    else {
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
    parma_particle->SetParameterName("particleName",true);
    parma_particle->SetDefaultValue("mu-");
    G4String candidateList = "mu- mu+";
//    G4int nPtcl = particleTable->entries();
//    for(G4int i=0;i<nPtcl;i++)
//    {
//        candidateList += particleTable->GetParticleName(i);
//        candidateList += " ";
//    }
    parma_particle->SetCandidates(candidateList);

    parma_position = new G4UIcmdWith3VectorAndUnit(parma_position_path.c_str(),this);
    parma_position->SetGuidance("Set starting position of the particle.");
    parma_position->SetParameterName("X","Y","Z",true,true);
    parma_position->SetDefaultUnit("cm");
    parma_position->SetUnitCandidates("micron mm cm m km");

}
