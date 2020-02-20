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

    cout<<"Height bins" << endl;
    cout<< fSettings->particlePredictor->fHist2DLow->fYbins->toString()<<endl;
    cout<< "ELectron"<<endl;
    cout<<"Energy bins"<<endl;
    cout<<fSettings->particlePredictor->fHist2DLow->fXbins->toString()<<endl;
    cout<<"Low hist"<<endl;
    cout<<fSettings->particlePredictor->fHist2DLow->dataToString()<<endl;
    cout<<"Energy bins"<<endl;
    cout<<fSettings->particlePredictor->fHist2DHight->fXbins->toString()<<endl;
    cout<<"Hight hist"<<endl;
    cout<<fSettings->particlePredictor->fHist2DHight->dataToString()<<endl;

    cout<< "Gamma"<<endl;
    cout<<"Energy bins"<<endl;
    cout<<fSettings->particlePredictor->fGammaHist2DLow->fXbins->toString()<<endl;
    cout<<"Low hist"<<endl;
    cout<<fSettings->particlePredictor->fGammaHist2DLow->dataToString()<<endl;
    cout<<"Energy bins"<<endl;
    cout<<fSettings->particlePredictor->fGammaHist2DHight->fXbins->toString()<<endl;
    cout<<"Hight hist"<<endl;
    cout<<fSettings->particlePredictor->fGammaHist2DHight->dataToString()<<endl;
    G4UserEventAction::EndOfEventAction(anEvent);
}


