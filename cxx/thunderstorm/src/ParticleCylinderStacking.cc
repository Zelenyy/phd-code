//
// Created by zelenyy on 03.02.2020.
//

#include "ParticleCylinderStacking.hh"

ParticleCylinderStacking::ParticleCylinderStacking(Settings *settings) : settings(settings) {
    cut = settings->born_cut;

    number = DataManager::instance()->createNamedNumber("particle_cylinder_number");
    for (auto it : settings->particle_cylinder_stacking){
        fouts[it] = DataFileManager::instance()->getDataFile<CylinderData>("particle_cylinder_" + it);
        number->number[it] = 0;
    }
    number->write_header(
            DataFileManager::instance()->getTextFile("particle_cylinder_number")
            );
}

G4ClassificationOfNewTrack ParticleCylinderStacking::ClassifyNewTrack(const G4Track * aTrack) {

    if (aTrack->GetKineticEnergy() < cut){
        return fKill;
    }
    auto name = aTrack->GetParticleDefinition()->GetParticleName();
    if (fouts.find(name) != fouts.end()){
        data.fillFromTrack(aTrack);
        fouts[name]->addData(data);
        number->number[name]++;
        return fKill;
    }

    return fUrgent;
}

void ParticleCylinderStacking::PrepareNewEvent() {
    G4UserStackingAction::PrepareNewEvent();
}
