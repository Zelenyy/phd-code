//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_EVENTACTION_HH
#define PHD_CODE_EVENTACTION_HH


#include <G4UserEventAction.hh>
#include "Data.hh"
#include "DataManager.hh"
#include "Settings.hh"
#include <iostream>

class EventAction : public G4UserEventAction {
public:
    EventAction(Settings* settings){
        dataManager = DataManager::instance();
        fSettings = settings;
    };

    void BeginOfEventAction(const G4Event *anEvent) override;

    void EndOfEventAction(const G4Event *anEvent) override;

private:
    DataManager * dataManager;
    Settings* fSettings;
};


#endif //PHD_CODE_EVENTACTION_HH
