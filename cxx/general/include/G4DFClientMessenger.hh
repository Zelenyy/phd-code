//
// Created by zelenyy on 22.11.2019.
//

#ifndef GEANT4_DFCLIENT_G4DFCLIENTMESSENGER_HH
#define GEANT4_DFCLIENT_G4DFCLIENTMESSENGER_HH

#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>
#include "G4UImessenger.hh"
#include "globals.hh"

class G4DFClient;

using namespace std;

class G4DFClientMessenger : public G4UImessenger {
public:
    G4DFClientMessenger(G4DFClient * client);

    ~G4DFClientMessenger() override;

    G4String GetCurrentValue(G4UIcommand *command) override;

    void SetNewValue(G4UIcommand *command, G4String newValue) override;

private:
    G4DFClient* dfClient;
private:
    G4UIdirectory * directory;
    G4UIcmdWithAString * project;
    G4UIcmdWithAString * gdml;
    G4UIcmdWithAnInteger * seed;

private:
    string df_directory = "/df/";
    string project_path = df_directory + "project";
    string gdml_path = df_directory + "gdml";
    string seed_path = df_directory + "seed";

};


#endif //GEANT4_DFCLIENT_G4DFCLIENTMESSENGER_HH
