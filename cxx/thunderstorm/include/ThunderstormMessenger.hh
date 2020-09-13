//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_THUNDERSTORMMESSENGER_HH
#define PHD_CODE_THUNDERSTORMMESSENGER_HH

#include <G4UIcmdWithAString.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>
#include <G4Server.hh>
#include "G4UImessenger.hh"
#include "PhysicsList.hh"
#include "Settings.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"

using namespace std;

class ThunderstormMessenger : public ServerMessenger {
public:

    explicit ThunderstormMessenger(Settings *pSettings);

    ~ThunderstormMessenger() override = default;

    G4String GetCurrentValue(G4UIcommand *command) override;

    void SetNewValue(G4UIcommand *command, G4String newValue) override;


private:
    Settings* settings;
    G4UIdirectory * thundestorm;
    G4UIcmdWithAString * physics;
    G4UIcmdWithAString * tracking;
    G4UIcmdWithAString * stackingParticle;
    G4UIcmdWithAString * detectorParticle;
    G4UIcmdWithADoubleAndUnit * minimalEnergy;
private:
    string thunderstorm_path = root_path + "thunderstorm/";
    string physics_path = thunderstorm_path + "physics";

    string tracking_path = thunderstorm_path + "tracking";
    string energy_cut_path = thunderstorm_path + "minimal_energy";
    string add_particle_stacking_path = thunderstorm_path + "addParticleInPCS";
    string add_particle_detector_path = thunderstorm_path + "addParticleInPD";

private:
    G4UIcmdWithAString* thunderstorm_gen_type;
    string thunderstorm_gen_type_path = thunderstorm_path + "geo_type";

    G4UIcmdWithADoubleAndUnit * geo_height;
    string geo_height_path = thunderstorm_path + "height";

    G4UIcmdWithADoubleAndUnit * field_z;
    string field_z_path = thunderstorm_path + "field_z";

    G4UIdirectory * aragats;
    string aragats_path = thunderstorm_path + "aragats/";

    G4UIcmdWithAString* aragats_geo_type;
    string aragats_geo_type_path = aragats_path + "geo_type";

    G4UIcmdWithABool* aragats_only_muon;
    string aragats_only_muon_path = aragats_path + "only_muon";

    G4UIdirectory * aragats_pie;
    string aragats_pie_path = aragats_path + "pie/";

    G4UIcmdWithADoubleAndUnit * pie_low;
    string pie_low_path = aragats_pie_path + "low";

    G4UIcmdWithADoubleAndUnit * pie_high;
    string pie_high_path = aragats_pie_path + "high";

    G4UIcmdWithADoubleAndUnit * pie_obs_lvl;
    string pie_obs_lvl_path = aragats_pie_path + "observed";

    G4UIdirectory * parma;
    string parma_path = thunderstorm_path + "parma/";

    G4UIcmdWithAString* parma_particle;
    string parma_particle_path = parma_path + "particle";

    G4UIcmdWith3VectorAndUnit* parma_position;
    string parma_position_path = parma_path + "position";

private:
    void initSteppingSettings();
    bool setSteppingSettings(G4UIcommand *command, G4String newValue);
    G4UIdirectory * stepping;
    G4UIcmdWithAString * stepping_type;
    string stepping_path = thunderstorm_path + "stepping/";
    string stepping_type_path = stepping_path  + "type";
private:
    void initStackingSettings();
    bool setStackingSettings(G4UIcommand *command, G4String newValue);
    G4UIdirectory * stacking;
    G4UIcmdWithAString * stacking_type;
    G4UIcmdWithABool* enableGamma;
    G4UIcmdWithABool* enablePositron;
    G4UIcmdWithABool* enableElectron;
    G4UIcmdWithABool* enableMuon;
    string stacking_path = thunderstorm_path + "stacking/";
    string stacking_type_path = stacking_path  + "type";
    string stacking_gamma_path = stacking_path  + "gamma";
    string stacking_electron_path = stacking_path  + "electron";
    string stacking_positron_path = stacking_path  + "positron";
    string stacking_muon_path = stacking_path  + "muon";

    G4UIcmdWithABool* saveGamma;
    G4UIcmdWithABool* saveElectron;
    G4UIcmdWithADoubleAndUnit * saveElectronCut;
    string stacking_gamma_save_path = stacking_path  + "save_gamma";
    string stacking_electron_save_path = stacking_path  + "save_electron";
    string stacking_electron_save_cut__path = stacking_path  + "save_electron_cut";
};


#endif //PHD_CODE_THUNDERSTORMMESSENGER_HH
