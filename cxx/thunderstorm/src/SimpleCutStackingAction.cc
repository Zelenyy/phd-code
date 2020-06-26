//
// Created by zelenyy on 03.02.2020.
//

#include <G4MuonPlus.hh>
#include <G4MuonMinus.hh>
#include "SimpleCutStackingAction.hh"
#include "G4MuonPlus.hh"
#include "G4MuonMinus.hh"

SimpleCutStackingAction::SimpleCutStackingAction(Settings *settings) {
    cut = settings->born_cut;
    only_muon = settings->aragatsSettings->only_muon;
}

G4ClassificationOfNewTrack SimpleCutStackingAction::ClassifyNewTrack(const G4Track * aTrack) {
    if (aTrack->GetKineticEnergy() < cut){
        return fKill;
    }
    if (only_muon){
        auto def = aTrack->GetDefinition();
        if (def == G4MuonPlus::Definition() or def == G4MuonMinus::Definition()){
            return fUrgent;
        }
        else{
            return fKill;
        }
    }
    return fUrgent;
}

void SimpleCutStackingAction::PrepareNewEvent() {
    G4UserStackingAction::PrepareNewEvent();
}



