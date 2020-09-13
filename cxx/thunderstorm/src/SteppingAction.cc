//
// Created by zelenyy on 10.02.2020.
//

#include "SteppingAction.hh"

void SteppingAction::UserSteppingAction(const G4Step *step) {
    G4UserSteppingAction::UserSteppingAction(step);
    if (step->GetTrack()->GetKineticEnergy() < fSettings->minimal_energy){
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
}




