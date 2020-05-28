//
// Created by zelenyy on 18.10.17.
//

#include <utility>
#include "SensitiveScoredDetector.hh"
#include "Logger.hh"

SensitiveScoredDetector::SensitiveScoredDetector(G4String name, Settings *settings) : G4VSensitiveDetector(std::move(name)), fSettings(settings) {
    deposit.resize(settings->number_of_cell);

}


void SensitiveScoredDetector::Initialize(G4HCofThisEvent *) {
    for (int i = 0; i< fSettings->number_of_cell; ++i) {
        deposit[i] = 0.0;
    }
}

G4bool SensitiveScoredDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) {
    auto volume = aStep->GetTrack()->GetVolume();
    if (volume->GetName() == "Cell_PV") {
        int id = volume->GetInstanceID();
        deposit[id] += aStep->GetTotalEnergyDeposit() / MeV;
//        cout << id << '\t' << aStep->GetTrack()->GetDefinition()->GetParticleName()<< '\t' << aStep->GetPreStepPoint()->GetWeight()<<'\t' << aStep->GetTotalEnergyDeposit() / MeV<<endl;
    }
//    cout << volume->GetInstanceID() << '\t' << volume->GetTranslation() << '\t'    << volume->GetName() << '\t' << endl;
    return 0;
}

void SensitiveScoredDetector::EndOfEvent(G4HCofThisEvent *) {
    if (fSettings->scoredDetectorMode == ScoredDetectorMode::single){
        auto run = DataSatellite::instance()->run;
        auto event = run->add_event();
        for (int i = 0; i< fSettings->number_of_cell; ++i) {
            event->add_deposit(deposit[i]);
        }
    }
    else if (fSettings->scoredDetectorMode == ScoredDetectorMode::sum){
        auto meanRun = DataSatellite::instance()->meanRun;
        auto mean = meanRun->mean();
        auto var = meanRun->variance();
        meanRun->set_number(meanRun->number() + 1);
        for (int i = 0; i< fSettings->number_of_cell; ++i) {
            meanRun->set_mean(i, meanRun->mean(i) + deposit[i]);
            meanRun->set_variance(i, meanRun->variance(i) + deposit[i]*deposit[i]);
        }
    }
}