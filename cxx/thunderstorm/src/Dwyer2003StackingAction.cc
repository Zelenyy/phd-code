//
// Created by zelenyy on 31.01.2020.
//

#include <G4Electron.hh>
#include <G4Positron.hh>
#include <G4Gamma.hh>

#include "Dwyer2003StackingAction.hh"

Dwyer2003StackingAction::Dwyer2003StackingAction(Settings *settings) {
    number = DataManager::instance()->createNumber("number");
    foutGamma = DataFileManager::instance()->getDataFile<CylinderIdData>("gamma");
    foutPositron = DataFileManager::instance()->getDataFile<CylinderIdData>("positron");
    cut = settings->born_cut;
    Logger::instance()->print("One generation set cut: " + to_string(cut) + " MeV");
    parent.reserve(100000);
}

G4ClassificationOfNewTrack Dwyer2003StackingAction::ClassifyNewTrack(const G4Track * aTrack) {
    auto indx = aTrack->GetTrackID();
    if (indx >= parent.size()){
        parent.resize(parent.size() + 100000);
    }
    parent[indx] = aTrack->GetDefinition()->GetParticleDefinitionID();

    if (aTrack->GetParentID() == 0){
        return fUrgent;
    }

    if (aTrack->GetKineticEnergy() < cut){
        return fKill;
    }

    const auto& position = aTrack->GetPosition();
    if (aTrack->GetDefinition() == G4Electron::Definition()){
        if (position.getZ() > 0){
            indx = aTrack->GetParentID();
            if (parent[indx] == G4Positron::Definition()->GetParticleDefinitionID()){
//                cout<< "Postron seed"<<endl;
                data.fillFromTrack(aTrack);
                foutPositron->addData(data);
                number->positron++;
                return fKill;
            }
            else if (parent[indx] == G4Gamma::Definition()->GetParticleDefinitionID()){
//                cout<< "Gamma seed"<<endl;
                data.fillFromTrack(aTrack);
                foutGamma->addData(data);
                number->gamma++;
                return fKill;
            }

        }
        if (aTrack->GetKineticEnergy() < 0.08*MeV){
            return fWaiting;
        }
        return fWaiting_3;
    }
    else if (aTrack->GetDefinition() == G4Positron::Definition()){
        return fWaiting_4;
    }
    else if (aTrack->GetDefinition() == G4Gamma::Definition()){
        return ClassifyGamma(aTrack);
    }
    return fUrgent;
}

void Dwyer2003StackingAction::PrepareNewEvent() {
    G4UserStackingAction::PrepareNewEvent();

}

G4ClassificationOfNewTrack Dwyer2003StackingAction::ClassifyGamma(const G4Track *aTrack) {
    if (aTrack->GetKineticEnergy() < 0.08*MeV){
        return fUrgent;
    }
    else if (aTrack->GetKineticEnergy() < 0.5*MeV){
        return fWaiting_1;
    }
    return fWaiting_2;
}

