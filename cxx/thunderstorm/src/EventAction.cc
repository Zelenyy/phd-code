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

    if (fSettings->particlePredictor != nullptr) {
        ofstream &fout = *DataFileManager::instance()->getTextFile("histDwyer2003");
        if (anEvent->GetEventID() == 0) {
            fout << "Histogram Descripiton" << endl;

            fout << "Start description" << endl;
            fout << "Type: low" << endl;
            fout << "Particle: " << "electron" << endl;
            fout << "X bins: energy (MeV)" << endl;
            fout << fSettings->particlePredictor->fHist2DLow->fXbins->toString() << endl;
            fout << "Y bins: height (meter)" << endl;
            fout << fSettings->particlePredictor->fHist2DLow->fYbins->toString() << endl;
            fout << "End description" << endl;

            fout << "Start description" << endl;
            fout << "Type: high" << endl;
            fout << "Particle: " << "electron" << endl;
            fout << "X bins: energy (MeV)" << endl;
            fout << fSettings->particlePredictor->fHist2DHight->fXbins->toString() << endl;
            fout << "Y bins: height (meter)" << endl;
            fout << fSettings->particlePredictor->fHist2DHight->fYbins->toString() << endl;
            fout << "End description" << endl;

            fout << "Start description" << endl;
            fout << "Type: low" << endl;
            fout << "Particle: " << "gamma" << endl;
            fout << "X bins: energy (MeV)" << endl;
            fout << fSettings->particlePredictor->fGammaHist2DLow->fXbins->toString() << endl;
            fout << "Y bins: height (meter)" << endl;
            fout << fSettings->particlePredictor->fGammaHist2DLow->fYbins->toString() << endl;
            fout << "End description" << endl;

            fout << "Start description" << endl;
            fout << "Type: high" << endl;
            fout << "Particle: " << "gamma" << endl;
            fout << "X bins: energy (MeV)" << endl;
            fout << fSettings->particlePredictor->fGammaHist2DHight->fXbins->toString() << endl;
            fout << "Y bins: height (meter)" << endl;
            fout << fSettings->particlePredictor->fGammaHist2DHight->fYbins->toString() << endl;
            fout << "End description" << endl;
        }

        fout << "Start event: " << anEvent->GetEventID() << endl;
        fout << "Electron" << endl;
        fout << "Low hist:" << endl;
        fout << fSettings->particlePredictor->fHist2DLow->dataToString() << endl;
        fout << "Electron" << endl;
        fout << "High hist:" << endl;
        fout << fSettings->particlePredictor->fHist2DHight->dataToString() << endl;
        fout << "Gamma" << endl;
        fout << "Low hist:" << endl;
        fout << fSettings->particlePredictor->fGammaHist2DLow->dataToString() << endl;
        fout << "Gamma" << endl;
        fout << "High hist:" << endl;
        fout << fSettings->particlePredictor->fGammaHist2DHight->dataToString() << endl;
        fout << "End event." << endl;
    }


    G4UserEventAction::EndOfEventAction(anEvent);
}


