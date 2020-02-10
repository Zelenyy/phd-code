//
// Created by zelenyy on 10.02.2020.
//

#ifndef PHD_CODE_STEPPINGACTION_HH
#define PHD_CODE_STEPPINGACTION_HH


#include <G4UserSteppingAction.hh>
#include "G4Step.hh"
#include "Settings.hh"

class SteppingAction : public G4UserSteppingAction {
public:
    explicit SteppingAction(Settings* settings);
    void UserSteppingAction(const G4Step *step) override;

private:
    bool energyCut = true;
    double energy_cut = 0.05*MeV;
    void EnergyCut(const G4Step *step);
};


#endif //PHD_CODE_STEPPINGACTION_HH
