//
// Created by zelenyy on 09.02.2020.
//

#include "TreeSocketSteppingAction.hh"


void TreeSocketSteppingAction::UserSteppingAction(const G4Step *step) {
    G4UserSteppingAction::UserSteppingAction(step);
    data.fillFromStep(step);
    socketOutput->addData(data);
}
