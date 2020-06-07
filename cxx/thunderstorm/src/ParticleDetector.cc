//
// Created by zelenyy on 03.02.2020.
//


#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <DataThunderstorm.hh>
#include "ParticleDetector.hh"

G4bool ParticleDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) {
    auto track = aStep->GetTrack();
    if (track->GetKineticEnergy() > cut){
        auto data = dataThunderstorm->particleDetectorList->add_data();
        dataThunderstorm->fillParticleDetector(data, aStep->GetTrack());
        if (dataThunderstorm->particleDetectorList->data_size() > 1000000){
            dataThunderstorm->saveParticleDetector();
        }
    }
    track->SetTrackStatus(fStopAndKill);
    return false;
}

ParticleDetector::ParticleDetector(G4String name, Settings *settings) : G4VSensitiveDetector(name) {
    Logger::instance()->print("Create particle detector: " + name);
    cut = settings->born_cut;
    dataThunderstorm = DataThunderstorm::instance();
    dataThunderstorm->initParticleDetector();
}
