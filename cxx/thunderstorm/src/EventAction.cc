//
// Created by zelenyy on 14.01.2020.
//

#include <G4Event.hh>
#include "EventAction.hh"
#include "Logger.hh"


void EventAction::BeginOfEventAction(const G4Event *anEvent) {
    cout << "\033[A\033[2K\r"; // Magic sequence for clear last line
    cout << "Start event: " << anEvent->GetEventID();
    dataManager->BeginEvent();
    G4UserEventAction::BeginOfEventAction(anEvent);
}

void EventAction::EndOfEventAction(const G4Event *anEvent) {
    if ((anEvent->GetEventID() + 1) % 100 == 0) {
        Logger::instance()->print("End event number: " + to_string(anEvent->GetEventID()));
    }
    dataManager->EndEvent();
    G4UserEventAction::EndOfEventAction(anEvent);
}


