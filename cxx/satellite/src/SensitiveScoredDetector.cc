//
// Created by zelenyy on 18.10.17.
//

#include <utility>
#include "SensitiveScoredDetector.hh"
#include "Logger.hh"

SensitiveScoredDetector::SensitiveScoredDetector(G4String name, Settings *settings) : G4VSensitiveDetector(std::move(name)), fSettings(settings) {
    run = DataSatellite::instance()->run;

//    for (int i = 0; i< fSettings->number_of_cell; ++i) {
//        data.energy[i] = 0.0;
//    }

//    auto storage = DataStorage::instance();
//    if (fSettings->scoredDetectorMode == single){
//        dataCell = storage->getMonolithDataCell<EnergyDepositData>("cellEnergyDeposit", fSettings->outputMode);
//    }

}


void SensitiveScoredDetector::Initialize(G4HCofThisEvent *) {
    event = run->add_event();

//    if (fSettings->scoredDetectorMode == single){
        for (int i = 0; i< fSettings->number_of_cell; ++i) {
//            data.energy[i] = 0.0;
            event->add_deposit(0.0);
        }
//    }

}

G4bool SensitiveScoredDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) {
    auto volume = aStep->GetTrack()->GetVolume();
    if (volume->GetName() == "Cell_PV") {
        int id = volume->GetInstanceID();
        auto dep = event->deposit(id);
        event->set_deposit(id, dep + aStep->GetTotalEnergyDeposit() / MeV);

//        cout << id << '\t' << aStep->GetTrack()->GetDefinition()->GetParticleName()<< '\t' << aStep->GetPreStepPoint()->GetWeight()<<'\t' << aStep->GetTotalEnergyDeposit() / MeV<<endl;
//        data.energy[id] += aStep->GetTotalEnergyDeposit() / MeV;
    }
//    cout << volume->GetInstanceID() << '\t' << volume->GetTranslation() << '\t'    << volume->GetName() << '\t' << endl;

    return 0;
}

void SensitiveScoredDetector::EndOfEvent(G4HCofThisEvent *) {

//    for (auto it = 0; it < 100; it++){
//        cout<< event->deposit(it) <<" ";
//    }
//    cout<<endl;

//    if (fSettings->scoredDetectorMode == single){
////        dataCell->addData(data);
////        if (fSettings->outputMode == file){
////            foutDeposit->addData(data);
////        }
////        if (fSettings->outputMode == socket_client){
////            socketOutput->addData(data);
////        }
//    }
//    else{
//
//    }

}