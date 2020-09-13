//
// Created by zelenyy on 14.01.2020.
//

#include <G4Event.hh>
#include "EventAction.hh"
#include "Logger.hh"
#include "DataThunderstorm.hh"

void EventAction::BeginOfEventAction(const G4Event *anEvent) {
    cout << "\033[A\033[2K\r"; // Magic sequence for clear last line
    cout << "Start event: " << anEvent->GetEventID()<<endl;
    dataFileManager->initializeEvent(anEvent->GetEventID());
    G4UserEventAction::BeginOfEventAction(anEvent);
}

void EventAction::EndOfEventAction(const G4Event *anEvent) {
    dataFileManager->finishEvent();
    if ((anEvent->GetEventID() + 1) % 10 == 0) {
        Logger::instance()->print("End event number: " + to_string(anEvent->GetEventID()));
    }

    if (fSettings->particlePredictor != nullptr) {
        auto list = fSettings->particlePredictor->toHistogram2DList();
        int size = list->ByteSize();
        auto fout = DataFileManager::instance()->getBinaryFile("histogram");
        fout->write(reinterpret_cast<char*>(&size), sizeof size);
        list->SerializeToOstream(fout);
        delete list;
    }

    G4UserEventAction::EndOfEventAction(anEvent);
}


