//
// Created by zelenyy on 03.02.2020.
//


#include <G4Gamma.hh>
#include <G4Electron.hh>
#include "ParticleDetector.hh"

G4bool ParticleDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) {
    auto track = aStep->GetTrack();
    if (track->GetKineticEnergy() > cut){
        auto name = track->GetParticleDefinition()->GetParticleName();
//        if (fouts.find(name) != fouts.end()){
//            data.fillFromTrack(track);
//            fouts[name]->addData(data);
//            number->number[name]++;
//        }
    }
    track->SetTrackStatus(fStopAndKill);
    return false;
}

ParticleDetector::ParticleDetector(G4String name, Settings *settings) : G4VSensitiveDetector(name) {
    cut = settings->born_cut;
//    number = DataManager::instance()->createNamedNumber("particle_detector_number");
//    for (auto it : settings->particle_detector){
//        fouts[it] = DataFileManager::instance()->getDataFile<CylinderData>("particle_detector_" + it);
//        number->number[it] = 0;
//    }
//    number->write_header(
//            DataFileManager::instance()->getTextFile("particle_detector_number")
//    );
}
