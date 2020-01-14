//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_EVENTACTION_HH
#define PHD_CODE_EVENTACTION_HH


#include <G4UserEventAction.hh>

class EventAction : public G4UserEventAction {
public:
    void BeginOfEventAction(const G4Event *anEvent) override;

    void EndOfEventAction(const G4Event *anEvent) override;
};


#endif //PHD_CODE_EVENTACTION_HH
