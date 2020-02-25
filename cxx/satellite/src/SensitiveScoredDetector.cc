//
// Created by zelenyy on 18.10.17.
//

#include <utility>

#include "SensitiveScoredDetector.hh"
#include "Logger.hh"

SensitiveScoredDetector::SensitiveScoredDetector(G4String name, Settings *settings) : G4VSensitiveDetector(std::move(name)), fSettings(settings) {
    for (int i = 0; i< fSettings->number_of_cell; ++i) {
        data.energy[i] = 0.0;
    }

    if (fSettings->scoredDetectorMode == single){
        if (fSettings->outputMode == file){
            foutDeposit = DataFileManager::instance()->getDataFile<EnergyDepositData>("cellEnergyDeposit");
            Logger::instance()->print("Files: cellEnergyDeposit");
            Logger::instance()->print("Structure: energy");
        }
        if (fSettings->outputMode == socket_client){
            socketOutput = DataFileManager::instance()->GetSocketOutput<EnergyDepositData>("energyDeposit");
            Logger::instance()->print("Socket: energyDeposit");
            Logger::instance()->print("Structure: energy");
        }
    }

}


void SensitiveScoredDetector::Initialize(G4HCofThisEvent *) {

    if (fSettings->scoredDetectorMode == single){
        for (int i = 0; i< fSettings->number_of_cell; ++i) {
            data.energy[i] = 0.0;
        }
    }

}

G4bool SensitiveScoredDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) {
    auto volume = aStep->GetTrack()->GetVolume();
    if (volume->GetName() == "Cell_PV") {
        int id = volume->GetInstanceID();
//        cout << id << '\t' << aStep->GetTrack()->GetDefinition()->GetParticleName()<< '\t' << aStep->GetPreStepPoint()->GetWeight()<<'\t' << aStep->GetTotalEnergyDeposit() / MeV<<endl;
        data.energy[id] += aStep->GetTotalEnergyDeposit() / MeV;
    }
//    cout << volume->GetInstanceID() << '\t' << volume->GetTranslation() << '\t'    << volume->GetName() << '\t' << endl;

    return 0;
}

void SensitiveScoredDetector::EndOfEvent(G4HCofThisEvent *) {
//    cout<<"eee"<<endl;
//    double edep = 0;
//    for (auto it = 0; it < 100; it++){
////        edep += data.energy[it];
//        cout<< data.energy[it] <<" ";
//    }
//    cout<<endl;
//    cout<<edep<<endl;
    if (fSettings->scoredDetectorMode == single){

        if (fSettings->outputMode == file){
            foutDeposit->addData(data);
        }
        if (fSettings->outputMode == socket_client){
            socketOutput->addData(data);
        }
    }
    else{

    }

}