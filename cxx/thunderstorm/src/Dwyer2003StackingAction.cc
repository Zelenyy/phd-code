//
// Created by zelenyy on 31.01.2020.
//

#include <G4Electron.hh>
#include <G4Positron.hh>
#include <G4Gamma.hh>
#include <Logger.hh>
#include "G4VProcess.hh"
#include "Dwyer2003StackingAction.hh"
#include "DataThunderstorm.hh"


/*
 *
Undefined -1
compt 29
conv 33
eBrem 18
eIoni 15
phot 25
 */
Dwyer2003StackingAction::Dwyer2003StackingAction(Settings *settings) {
    cut = settings->minimal_energy;
    Logger::instance()->print("One generation set cut: " + to_string(cut) + " MeV");
    fParticlePredictor = settings->particlePredictor;
    dataFileManager = DataFileManager::instance();
    gammaData = new CylinderId();
    positronData = new CylinderId;
    dataFileManager->registerDataContainer("gammaSeed", gammaData);
    dataFileManager->registerDataContainer("positronSeed", positronData);
//    temp = &DataFileManager::instance()->models;

}

G4ClassificationOfNewTrack Dwyer2003StackingAction::ClassifyNewTrack(const G4Track * aTrack) {

    if (aTrack->GetParentID() == 0){
        return fUrgent;
    }

    if (aTrack->GetKineticEnergy() < cut){
        return fKill;
    }

    const auto& position = aTrack->GetPosition();
    if (aTrack->GetDefinition() == G4Electron::Definition()){
        if (position.getZ() > 0) {
            auto modelId = aTrack->GetCreatorModelID();
            if (modelId == 25 or modelId == 29 or modelId == 33) {
                gammaData->addTrack(aTrack);
                return fKill;
            } else {
                int indx = aTrack->GetParentID();
                if (positronIndx.find(indx) != positronIndx.end()) {
                    positronData->addTrack(aTrack);
                    return fKill;
                }
            }

            if (!fParticlePredictor->accept(aTrack)){
                return fKill;
            }

            if (aTrack->GetKineticEnergy() < 0.08 * MeV) {
                return fWaiting;
            }
            return fWaiting_3;
        }
        if (!fParticlePredictor->accept(aTrack)){
            return fKill;
        }
    }
    else if (aTrack->GetDefinition() == G4Positron::Definition()){
        positronIndx.insert(aTrack->GetTrackID());
        return fWaiting_4;
    }
    else if (aTrack->GetDefinition() == G4Gamma::Definition()){
        return ClassifyGamma(aTrack);
    }
    return fUrgent;
}

void Dwyer2003StackingAction::PrepareNewEvent() {
    G4UserStackingAction::PrepareNewEvent();
        positronIndx.clear();
}

G4ClassificationOfNewTrack Dwyer2003StackingAction::ClassifyGamma(const G4Track *aTrack) {
    if (!fParticlePredictor->accept(aTrack)){
        return fKill;
    }
    if (aTrack->GetKineticEnergy() < 0.08*MeV){
        return fUrgent;
    }
    else if (aTrack->GetKineticEnergy() < 0.5*MeV){
        return fWaiting_1;
    }
    return fWaiting_2;
}

