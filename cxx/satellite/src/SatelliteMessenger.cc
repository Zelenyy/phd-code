//
// Created by zelenyy on 14.01.2020.
//

#include "SatelliteMessenger.hh"

G4String SatelliteMessenger::GetCurrentValue(G4UIcommand *command) {
    return G4UImessenger::GetCurrentValue(command);
}

void SatelliteMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
    if (command == detector) {
        if (newValue == "sum") {
            settings->scoredDetectorMode = ScoredDetectorMode::sum;
        } else if (newValue == "single") {
            settings->scoredDetectorMode = ScoredDetectorMode::single;
        }
    } else if (command == output) {
        if (newValue == "file") {
            settings->outputMode = OutputMode ::file;
        } else if (newValue == "socket") {
            settings->outputMode = OutputMode ::socket_client;
        }
    } else {
        G4UImessenger::SetNewValue(command, newValue);
    }
}

SatelliteMessenger::SatelliteMessenger(Settings *pSettings) : settings(pSettings) {
    directory = new G4UIdirectory(satellite_directory.c_str());
    directory->SetGuidance("This is helper");

    detector = new G4UIcmdWithAString(detector_mode.c_str(), this);
    detector->SetGuidance("Set detector mode");
    detector->SetParameterName("mode", true);
    detector->SetDefaultValue("sum");
    detector->SetCandidates("sum single");

    output = new G4UIcmdWithAString(output_mode.c_str(), this);
    output->SetGuidance("Set output mode");
    output->SetParameterName("mode", true);
    output->SetDefaultValue("file");
    output->SetCandidates("file socket");

}
