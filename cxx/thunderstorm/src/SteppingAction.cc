//
// Created by zelenyy on 10.02.2020.
//

#include "SteppingAction.hh"

void SteppingAction::UserSteppingAction(const G4Step *step) {
    G4UserSteppingAction::UserSteppingAction(step);
    if (energyCut){
        EnergyCut(step);
    }

}

SteppingAction::SteppingAction(Settings *settings) {
    energy_cut = settings->born_cut;
    energyCut = settings->stepping_energy_cut;
}

void SteppingAction::EnergyCut(const G4Step *step) {
    if (step->GetTrack()->GetKineticEnergy() < energy_cut){
        step->GetTrack()->SetTrackStatus(fStopAndKill);
    }
}

