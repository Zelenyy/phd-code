//
// Created by zelenyy on 01.07.2020.
//

#ifndef PHD_CODE_CONCENTRATIONSTEPPINGACTION_HH
#define PHD_CODE_CONCENTRATIONSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"

class ConcentrationSteppingAction : public G4UserSteppingAction {
public:
    ConcentrationSteppingAction();
    void UserSteppingAction(const G4Step *step) override;

private:
    DataThunderstorm *data;
};


#endif //PHD_CODE_CONCENTRATIONSTEPPINGACTION_HH
