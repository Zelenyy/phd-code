//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_THUNDERSTORMMESSENGER_HH
#define PHD_CODE_THUNDERSTORMMESSENGER_HH

#include <G4UIcmdWithAString.hh>
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
private:
    string thunderstorm_directory = "/thunderstorm/";
    string physics_path = thunderstorm_directory + "physics";
    string stacking_path = thunderstorm_directory + "stacking";
};


#endif //PHD_CODE_THUNDERSTORMMESSENGER_HH
