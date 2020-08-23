//
// Created by zelenyy on 03.02.2020.
//

#include <G4MuonPlus.hh>
#include <G4MuonMinus.hh>
#include <G4Positron.hh>
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "StackingAction.hh"

StackingAction::StackingAction(Settings *settings) : fSettings(settings){
    stackingSettings = fSettings->stackingSettings;
    cut = settings->born_cut;
    only_muon = settings->aragatsSettings->only_muon;
}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track * aTrack) {
    if (aTrack->GetParentID() == 0){
        return fUrgent;
    }

    if (aTrack->GetKineticEnergy() < cut){
        return fKill;
    }
    auto def = aTrack->GetDefinition();
    if (def == G4MuonPlus::Definition() or def == G4MuonMinus::Definition()){
        return ClassifyMuon(aTrack);
    }
    if (def == G4Gamma::Definition()){
        return ClassifyGamma(aTrack);
    }
    if (def == G4Electron::Definition()){
        return ClassifyElectron(aTrack);
    }
    if (def == G4Positron::Definition()){
        return ClassifyPositron(aTrack);
    }
    return fUrgent;
}

void StackingAction::PrepareNewEvent() {
    G4UserStackingAction::PrepareNewEvent();
}

G4ClassificationOfNewTrack StackingAction::ClassifyGamma(const G4Track * aTrack) {
    if (stackingSettings->disableGamma){
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

G4ClassificationOfNewTrack StackingAction::ClassifyElectron(const G4Track * aTrack) {
    if (aTrack->GetKineticEnergy() < 0.08 * MeV) {
        return fWaiting;
    }
    return fWaiting_3;
}

G4ClassificationOfNewTrack StackingAction::ClassifyPositron(const G4Track * aTrack) {
    if (stackingSettings->disablePositron){
        return fKill;
    }
    return fWaiting_4;
}

G4ClassificationOfNewTrack StackingAction::ClassifyMuon(const G4Track * aTrack) {
    if (stackingSettings->disableMuon){
        return fKill;
    }
    return fWaiting_4;
}



