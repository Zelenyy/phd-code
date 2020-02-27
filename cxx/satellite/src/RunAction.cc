//
// Created by zelenyy on 26.02.2020.
//

#include <DataStorage.hh>
#include "RunAction.hh"

void RunAction::EndOfRunAction(const G4Run *aRun) {
    auto storage = DataStorage::instance();
    storage->endRun();
    G4UserRunAction::EndOfRunAction(aRun);
}
