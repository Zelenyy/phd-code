//
// Created by zelenyy on 22.08.2020.
//

#ifndef PHD_CODE_SERVERSETTINGS_HH
#define PHD_CODE_SERVERSETTINGS_HH

enum class GeometryType {
    gdml,
    custom
};
enum class PrimaryGeneratorType {
    gps,
    custom
};

class ServerSettings {
public:
    GeometryType geometry = GeometryType::gdml;
    string gdml = "default.gdml";
    PrimaryGeneratorType generator = PrimaryGeneratorType::gps;
    long seed = -1;
    int numberOfAdditionalWaitingStacks = 4;
    int lengthOfCommand = 1000;
    bool visualization = false;
    int argc = 0;
    char** argv;
};

#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithAnInteger.hh>
#include <random>
#include <G4UImanager.hh>
#include <G4VisManager.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>
#include "G4RunManager.hh"
#include "GeneralParticleSource.hh"
#include "G4UIcmdWithABool.hh"
#include "DetectorConstruction.hh"

#endif //PHD_CODE_SERVERSETTINGS_HH
