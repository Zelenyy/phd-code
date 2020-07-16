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

    ThunderstormMessenger(Settings *pSettings);

    ~ThunderstormMessenger() override = default;

    G4String GetCurrentValue(G4UIcommand *command) override;

    void SetNewValue(G4UIcommand *command, G4String newValue) override;
private:
    G4ParticleTable* particleTable;
    Settings* settings;
    G4UIdirectory * thundestorm;
    G4UIcmdWithAString * physics;
    G4UIcmdWithAString * stacking;
    G4UIcmdWithAString * stepping;
    G4UIcmdWithAString * tracking;
    G4UIcmdWithAString * stackingParticle;
    G4UIcmdWithAString * detectorParticle;
    G4UIcmdWithADoubleAndUnit * energyCut;
private:
    string thunderstorm_path = root_path + "thunderstorm/";
    string physics_path = thunderstorm_path + "physics";
    string stacking_path = thunderstorm_path + "stacking";
    string stepping_path = thunderstorm_path + "stepping";
    string tracking_path = thunderstorm_path + "tracking";
    string cut_path = thunderstorm_path + "cut/";
    string energy_cut_path = cut_path +"energy";
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

};


#endif //PHD_CODE_THUNDERSTORMMESSENGER_HH
