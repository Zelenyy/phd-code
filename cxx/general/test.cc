//
// Created by zelenyy on 22.11.2019.
//

#include <G4DFClient.hh>
#include <G4UImanager.hh>
#include <FTFP_BERT.hh>

int main(int argc, char **argv){
    G4DFClient* dfClient = G4DFClient::instance();
    fstream fin;
    fin.open("init.mac");
    dfClient->read(fin);
    dfClient->setup(new FTFP_BERT, nullptr);
    dfClient->initialize();
    dfClient->read(fin);
    fin.close();
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/run/beamOn 1");
//    UImanager->ApplyCommand("/df/project test");
    cout<<"Project: "<<dfClient->getProject()<<endl;
    dfClient->stop();
    return 0;
}
