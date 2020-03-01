//
// Created by zelenyy on 26.02.2020.
//

#include <DataSatellite.hh>
#include <fstream>
#include <Logger.hh>
#include <SocketOutput.hh>
#include "RunAction.hh"

void RunAction::BeginOfRunAction(const G4Run *aRun) {
    auto dataSatellite = DataSatellite::instance();

    dataSatellite->run->Clear();

    G4UserRunAction::BeginOfRunAction(aRun);
}

void RunAction::EndOfRunAction(const G4Run *aRun) {
    auto dataSatellite = DataSatellite::instance();
 if (fSettings->outputMode == OutputMode::file){
     Logger::instance()->print("Write to deposit.proto.bin");
     ofstream fout;
     fout.open("deposit.proto.bin");
     dataSatellite->run->SerializeToOstream(&fout);
     fout.flush();
     fout.close();
 } else if (fSettings->outputMode == OutputMode::socket_client){

     auto socket  =  new SocketOutput("deposit", 8777);
        int socketId = socket->getID();
        auto data =  dataSatellite->run->SerializeAsString();
        socket->write(data);
        socket->closeSocket();
         delete socket;
 }
    G4UserRunAction::EndOfRunAction(aRun);
}


