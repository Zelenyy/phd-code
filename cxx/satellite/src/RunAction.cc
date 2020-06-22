//
// Created by zelenyy on 26.02.2020.
//

#include <DataSatellite.hh>
#include <fstream>
#include <Logger.hh>

#include "RunAction.hh"
#include "G4RunManager.hh"
void RunAction::BeginOfRunAction(const G4Run *aRun) {
    auto dataSatellite = DataSatellite::instance();


    if (fSettings->scoredDetectorMode == ScoredDetectorMode::sum){
        auto meanRun = dataSatellite->meanRun;
        meanRun->Clear();
        for (int i = 0; i< fSettings->number_of_cell; ++i) {
            meanRun->add_mean(0.0);
            meanRun->add_variance(0.0);
        }
        meanRun->set_number(0);
    }
    else{
        dataSatellite->run->Clear();
    }



    G4UserRunAction::BeginOfRunAction(aRun);
}

void RunAction::EndOfRunAction(const G4Run *aRun) {
    auto dataSatellite = DataSatellite::instance();
    if (fSettings->scoredDetectorMode == ScoredDetectorMode::sum) {
        auto run = dataSatellite->meanRun;
        auto mean = run->mean();
        auto var = run->variance();
        for (int i = 0; i< fSettings->number_of_cell; ++i) {
            run->set_mean(i, run->mean(i)/run->number());
            run->set_variance(i, run->variance(i)/run->number() - run->mean(i)*run->mean(i));
        }
    }


 if (fSettings->outputMode == OutputMode::file){
     Logger::instance()->print("Write to deposit.proto.bin");
     ofstream fout;
     fout.open("deposit.proto.bin");
     if (fSettings->scoredDetectorMode == ScoredDetectorMode::single) {
         dataSatellite->run->SerializeToOstream(&fout);
     }
     else if (fSettings->scoredDetectorMode == ScoredDetectorMode::sum) {
         dataSatellite->meanRun->SerializeToOstream(&fout);
     }
     fout.flush();
     fout.close();
 } else if (fSettings->outputMode == OutputMode::socket_client){
     string data;
     if (fSettings->scoredDetectorMode == ScoredDetectorMode::single) {
         data = dataSatellite->run->SerializeAsString();
     }
     else if (fSettings->scoredDetectorMode == ScoredDetectorMode::sum) {
         data = dataSatellite->meanRun->SerializeAsString();
     }
     auto size = static_cast<unsigned long int>(data.length());
     socket->write(size);
     socket->write(data);
     Logger::instance()->print("Write "+to_string(size) +" bytes to socket on address " + to_string(fSettings->port));
 }
    G4UserRunAction::EndOfRunAction(aRun);
}


