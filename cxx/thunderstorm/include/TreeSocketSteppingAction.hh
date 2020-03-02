//
// Created by zelenyy on 09.02.2020.
//

#ifndef PHD_CODE_TREESOCKETSTEPPINGACTION_HH
#define PHD_CODE_TREESOCKETSTEPPINGACTION_HH


#include <G4UserSteppingAction.hh>
#include <DataFileManager.hh>
#include "G4Step.hh"
#include "Data.hh"

class TreeSocketSteppingAction : public G4UserSteppingAction {
public:
    TreeSocketSteppingAction(){
    };
    void UserSteppingAction(const G4Step *step) override;

private:
    TreeSocket data;
};


#endif //PHD_CODE_TREESOCKETSTEPPINGACTION_HH