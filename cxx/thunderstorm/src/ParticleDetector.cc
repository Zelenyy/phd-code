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
        data->addTrack(track);
    }
    track->SetTrackStatus(fStopAndKill);
    return false;
}

ParticleDetector::ParticleDetector(G4String name, Settings *settings) : G4VSensitiveDetector(name) {
    Logger::instance()->print("Create particle detector: " + name);
    cut = settings->born_cut;
    dataFileManager = DataFileManager::instance();
    data = new ParticleDetectorList();
    dataFileManager->registerDataContainer("particle_detector", data);

}
