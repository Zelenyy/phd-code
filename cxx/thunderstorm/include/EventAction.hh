//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_EVENTACTION_HH
#define PHD_CODE_EVENTACTION_HH


#include <G4UserEventAction.hh>
#include "Settings.hh"
#include "DataThunderstorm.hh"
#include <iostream>
#include <DataFileManager.hh>

class EventAction : public G4UserEventAction {
public:
    explicit EventAction(Settings* settings): G4UserEventAction(), fSettings(settings){
        dataFileManager = DataFileManager::instance();
    };

    void BeginOfEventAction(const G4Event *anEvent) override;

    void EndOfEventAction(const G4Event *anEvent) override;


public:
    ElectronsCounter* electronCounter = nullptr;
private:
    DataFileManager* dataFileManager;
    Settings* fSettings;
};


#endif //PHD_CODE_EVENTACTION_HH
