//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_SATELLITEMESSENGER_HH
#define PHD_CODE_SATELLITEMESSENGER_HH

#include <G4UIcmdWithAString.hh>
#include "G4UIcmdWithAnInteger.hh"
#include <G4UIcmdWithADoubleAndUnit.hh>
#include "G4UImessenger.hh"
#include "Settings.hh"

using namespace std;

class SatelliteMessenger : public G4UImessenger {
public:

    SatelliteMessenger(Settings *pSettings);

    ~SatelliteMessenger() override = default;

    G4String GetCurrentValue(G4UIcommand *command) override;

    void SetNewValue(G4UIcommand *command, G4String newValue) override;
private:
    Settings* settings;
    G4UIdirectory * directory;
    G4UIcmdWithAString * physics;
    G4UIcmdWithAString * detector;
    G4UIcmdWithAString * output;
    G4UIcmdWithAnInteger * port;
private:
    string satellite_directory = "/satellite/";
    string detector_mode = satellite_directory + "detector";
    string output_mode = satellite_directory + "output";
    string port_path = satellite_directory + "port";
};


#endif //PHD_CODE_SATELLITEMESSENGER_HH
