//
// Created by zelenyy on 23.08.2020.
//

#ifndef PHD_CODE_RUNACTION_HH
#define PHD_CODE_RUNACTION_HH

#include "G4UserRunAction.hh"

class RunAction: public G4UserRunAction{
public:
    explicit RunAction(Settings* settings): G4UserRunAction() , fSettings(settings){
        dataFileManager = DataFileManager::instance();
    };

    void BeginOfRunAction(const G4Run *aRun) override {
        G4UserRunAction::BeginOfRunAction(aRun);
        dataFileManager->initializeRun();
    }

    void EndOfRunAction(const G4Run *aRun) override {
        G4UserRunAction::EndOfRunAction(aRun);
        dataFileManager->finishRun();
    }

private:
    DataFileManager* dataFileManager;
    Settings* fSettings;
};

#endif //PHD_CODE_RUNACTION_HH
