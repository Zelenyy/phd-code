//
// Created by zelenyy on 03.02.2020.
//

#include "SimpleCutStackingAction.hh"

SimpleCutStackingAction::SimpleCutStackingAction(Settings *settings) {
    cut = settings->born_cut;
}

G4ClassificationOfNewTrack SimpleCutStackingAction::ClassifyNewTrack(const G4Track * aTrack) {
    if (aTrack->GetKineticEnergy() < cut){
        return fKill;
    }
    return fUrgent;
}

void SimpleCutStackingAction::PrepareNewEvent() {
    G4UserStackingAction::PrepareNewEvent();
}



