//
// Created by zelenyy on 01.07.2020.
//

#include <DataThunderstorm.hh>
#include "ConcentrationSteppingAction.hh"

void ConcentrationSteppingAction::UserSteppingAction(const G4Step *step) {

    G4UserSteppingAction::UserSteppingAction(step);
}

ConcentrationSteppingAction::ConcentrationSteppingAction() {
    data = DataThunderstorm::instance();
    data->initConcentration();
}
