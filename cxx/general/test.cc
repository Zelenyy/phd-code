//
// Created by zelenyy on 22.11.2019.
//

#include <G4UImanager.hh>
#include "fstream"
#include <FTFP_BERT.hh>

using namespace std;

int main(int argc, char **argv){
    fstream fin;
    fin.open("init.mac");
    fin.close();
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    UImanager->ApplyCommand("/run/beamOn 1");
    return 0;
}
