//
// Created by zelenyy on 31.01.2020.
//

#include <G4Electron.hh>
#include <G4Positron.hh>
#include <G4Gamma.hh>
#include "G4VProcess.hh"
#include "Dwyer2003StackingAction.hh"
#include "DataThunderstorm.hh"


/*
 *
Undefined -1
compt 29
conv 33
eBrem 18
eIoni 15
phot 25
 */
Dwyer2003StackingAction::Dwyer2003StackingAction(Settings *settings) {
//    number = DataManager::instance()->createNumber("number");
//    foutGamma = DataFileManager::instance()->getDataFile<CylinderIdData>("gamma");
//    foutPositron = DataFileManager::instance()->getDataFile<CylinderIdData>("positron");
    cut = settings->born_cut;
    Logger::instance()->print("One generation set cut: " + to_string(cut) + " MeV");
    fParticlePredictor = settings->particlePredictor;
    auto dataThunderstorm = DataThunderstorm::instance();
    dataThunderstorm->initDwyer2003StackingActionData();
    gammaData = dataThunderstorm->gammaData;
    positronData = dataThunderstorm->positronData;

//    temp = &DataFileManager::instance()->models;

}

G4ClassificationOfNewTrack Dwyer2003StackingAction::ClassifyNewTrack(const G4Track * aTrack) {


    if (aTrack->GetParentID() == 0){
        return fUrgent;
    }

    if (aTrack->GetKineticEnergy() < cut){
        return fKill;
    }



    const auto& position = aTrack->GetPosition();
    if (aTrack->GetDefinition() == G4Electron::Definition()){
        if (position.getZ() > 0) {
            auto modelId = aTrack->GetCreatorModelID();
            if (modelId == 25 or modelId == 29 or modelId == 33) {
//                cout<< "Gamma seed"<<endl;
                auto cylinderId = gammaData->add_cylinderid();
                DataThunderstorm::fillCylinderId(cylinderId, aTrack);
//                data.fillFromTrack(aTrack);
//                foutGamma->addData(data);
//                number->gamma++;
                return fKill;
            } else {
                int indx = aTrack->GetParentID();
//                cout<<"Electron "<<indx<<endl;
                if (positronIndx.find(indx) != positronIndx.end()) {
//                cout<< "Postron seed"<<endl;
                    auto cylinderId = positronData->add_cylinderid();
                    DataThunderstorm::fillCylinderId(cylinderId, aTrack);
//                    data.fillFromTrack(aTrack);
//                    foutPositron->addData(data);
//                    number->positron++;
                    return fKill;
                }
            }

            if (!fParticlePredictor->accept(aTrack)){
                return fKill;
            }

            if (aTrack->GetKineticEnergy() < 0.08 * MeV) {
                return fWaiting;
            }
            return fWaiting_3;
        }
        if (!fParticlePredictor->accept(aTrack)){
            return fKill;
        }
    }
    else if (aTrack->GetDefinition() == G4Positron::Definition()){
//        cout<<"Positron "<<aTrack->GetTrackID()<<endl;
        positronIndx.insert(aTrack->GetTrackID());
        return fWaiting_4;
    }
    else if (aTrack->GetDefinition() == G4Gamma::Definition()){
        return ClassifyGamma(aTrack);
    }
    return fUrgent;
}

void Dwyer2003StackingAction::PrepareNewEvent() {
    G4UserStackingAction::PrepareNewEvent();
        positronIndx.clear();
        gammaData->Clear();
        positronData->Clear();
        eventId++;

        gammaData->set_eventid(eventId);
        positronData->set_eventid(eventId);
}

G4ClassificationOfNewTrack Dwyer2003StackingAction::ClassifyGamma(const G4Track *aTrack) {
    if (!fParticlePredictor->accept(aTrack)){
        return fKill;
    }
    if (aTrack->GetKineticEnergy() < 0.08*MeV){
        return fUrgent;
    }
    else if (aTrack->GetKineticEnergy() < 0.5*MeV){
        return fWaiting_1;
    }
    return fWaiting_2;
}

