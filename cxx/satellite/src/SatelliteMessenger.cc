//
// Created by zelenyy on 14.01.2020.
//

#include "SatelliteMessenger.hh"

G4String SatelliteMessenger::GetCurrentValue(G4UIcommand *command) {
    return ServerMessenger::GetCurrentValue(command);
}

void SatelliteMessenger::SetNewValue(G4UIcommand *command, G4String newValue) {
    if (setGeometrySettings(command, newValue)){
        return;
    }

    if (command == detector) {
        if (newValue == "sum") {
            settings->scoredDetectorMode = ScoredDetectorMode::sum;
        } else if (newValue == "single") {
            settings->scoredDetectorMode = ScoredDetectorMode::single;
        }
    } else if (command == output) {
        if (newValue == "file") {
            settings->outputMode = OutputMode::file;
        } else if (newValue == "socket") {
            settings->outputMode = OutputMode::socket_client;
        }
    } else if (command == port) {
        settings->port = G4UIcmdWithAnInteger::GetNewIntValue(newValue);
    } else {
        ServerMessenger::SetNewValue(command, newValue);
    }
}

SatelliteMessenger::SatelliteMessenger(Settings *pSettings) : ServerMessenger(pSettings), settings(pSettings) {
    satellite = new G4UIdirectory(satellite_directory.c_str());
    satellite->SetGuidance("This is helper");

    initGeometrySettings();

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

    port = new G4UIcmdWithAnInteger(port_path.c_str(), this);
    output->SetParameterName("port", false);

}

void SatelliteMessenger::initGeometrySettings() {
    geometry = new G4UIdirectory(geometry_path.c_str());
    satellite_geo_type = new G4UIcmdWithAString(satellite_geo_type_path.c_str(), this);
    satellite_geo_type->SetParameterName("type", false);
}

bool SatelliteMessenger::setGeometrySettings(G4UIcommand *command, G4String newValue) {
    auto geoSettings = settings->geometrySettings;
    if (command == satellite_geo_type) {
        if (newValue == "tyvek"){
            geoSettings->geometryType = SatelliteSubType::tyvek;
        }
    } else{
        return false;
    }
    return true;
}
