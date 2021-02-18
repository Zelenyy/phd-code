//
// Created by zelenyy on 10.02.2020.
//

#include "SteppingAction.hh"

void SteppingAction::UserSteppingAction(const G4Step *step) {
    G4UserSteppingAction::UserSteppingAction(step);

    auto def = step->GetTrack()->GetDefinition();
    if (fSettings->superviseTree){
        if (def == G4Gamma::Definition()){
            superviseTree->stepping(step);
        }
        if (def == G4Electron::Definition()){
            electronsCounter->stepping(step);
        }
    }



    if (step->GetTrack()->GetKineticEnergy() < fSettings->minimal_energy){
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
}




