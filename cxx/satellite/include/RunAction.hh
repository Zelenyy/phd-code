//
// Created by zelenyy on 26.02.2020.
//

#ifndef PHD_CODE_RUNACTION_HH
#define PHD_CODE_RUNACTION_HH


#include <G4UserRunAction.hh>

class RunAction: public G4UserRunAction {
public:
    void EndOfRunAction(const G4Run *aRun) override;

};


#endif //PHD_CODE_RUNACTION_HH
