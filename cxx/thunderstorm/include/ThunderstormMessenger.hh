//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_THUNDERSTORMMESSENGER_HH
#define PHD_CODE_THUNDERSTORMMESSENGER_HH

#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include "G4UImessenger.hh"
#include "PhysicsList.hh"
#include "Settings.hh"

using namespace std;

class ThunderstormMessenger : public G4UImessenger {
public:

    ThunderstormMessenger(Settings *pSettings);

    ~ThunderstormMessenger() override = default;

    G4String GetCurrentValue(G4UIcommand *command) override;

    void SetNewValue(G4UIcommand *command, G4String newValue) override;
private:
    Settings* settings;
    G4UIdirectory * directory;
    G4UIcmdWithAString * physics;
    G4UIcmdWithAString * stacking;
    G4UIcmdWithAString * stackingParticle;
    G4UIcmdWithAString * detectorParticle;
    G4UIcmdWithADoubleAndUnit * energyCut;
private:
    string thunderstorm_directory = "/thunderstorm/";
    string physics_path = thunderstorm_directory + "physics";
    string stacking_path = thunderstorm_directory + "stacking";
    string cut_path = thunderstorm_directory +"cut/";
    string energy_cut_path = cut_path +"energy";
    string add_particle_stacking_path = thunderstorm_directory + "addParticleInPCS";
    string add_particle_detector_path = thunderstorm_directory + "addParticleInPD";
};


#endif //PHD_CODE_THUNDERSTORMMESSENGER_HH
