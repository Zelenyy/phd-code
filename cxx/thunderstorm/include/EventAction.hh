//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_EVENTACTION_HH
#define PHD_CODE_EVENTACTION_HH


#include <G4UserEventAction.hh>
#include "Data.hh"
#include "DataManager.hh"

class EventAction : public G4UserEventAction {
public:
    EventAction(){
        dataManager = DataManager::instance();
    };

    void BeginOfEventAction(const G4Event *anEvent) override;

    void EndOfEventAction(const G4Event *anEvent) override;

private:
    DataManager * dataManager;
};


#endif //PHD_CODE_EVENTACTION_HH
