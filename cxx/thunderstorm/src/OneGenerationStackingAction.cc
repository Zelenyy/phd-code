//
// Created by zelenyy on 14.01.2020.
//


#include "OneGenerationStackingAction.hh"
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <DataFileManager.hh>
#include "G4Positron.hh"
#include "DataManager.hh"

OneGenerationStackingAction::OneGenerationStackingAction(Settings *settings) {
    number = DataManager::instance()->createNumber("number");
    foutGamma = DataFileManager::instance()->getDataFile<CylinderData>("gamma");
    foutElectron = DataFileManager::instance()->getDataFile<CylinderData>("electron");
    foutPositron = DataFileManager::instance()->getDataFile<CylinderData>("positron");
    cut = settings->born_cut;
}

G4ClassificationOfNewTrack OneGenerationStackingAction::ClassifyNewTrack(const G4Track *aTrack) {

    if (aTrack->GetParentID() == 0) {
        primaryParticle = aTrack->GetDefinition();
        return fUrgent;
    } else {
        data.fillFromTrack(aTrack);
        if (aTrack->GetDefinition() == G4Electron::Definition()) {
            foutElectron->addData(data);
            number->electron++;
            return fKill;
        }
        if (aTrack->GetDefinition() == G4Gamma::Definition()) {
            foutGamma->addData(data);
            number->gamma++;
            return fKill;
        }
        if (aTrack->GetDefinition() == G4Positron::Definition()) {
            foutPositron->addData(data);
            number->positron++;
            return fKill;
        }
    }


    return G4UserStackingAction::ClassifyNewTrack(aTrack);
}

void OneGenerationStackingAction::PrepareNewEvent() {
//    if (flag) {
//        number.write(foutNumber);
//    }
//    number.clear();
//    flag = true;
    G4UserStackingAction::PrepareNewEvent();
}

OneGenerationStackingAction::~OneGenerationStackingAction() {
//    number.write(foutNumber);
//    foutNumber->flush();
}
