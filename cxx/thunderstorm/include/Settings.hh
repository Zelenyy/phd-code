//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_SETTINGS_HH
#define PHD_CODE_SETTINGS_HH

#include <G4Server.hh>
#include "G4SystemOfUnits.hh"
#include "ParticlePredictor.hh"
#include "G4MuonMinus.hh"
#include "ParticleField.hh"
#include "ServerSettings.hh"

using namespace std;


enum class ThunderstormSubType {
    uniform_cylinder,
    aragats
};

struct GeometrySettings {
    double height = 0.0 * meter;
    double field_z = 0.0 * kilovolt / meter;
    double radius = 4 * kilometer;
    double cloud_length = 1000*meter;
    ThunderstormSubType geometryType = ThunderstormSubType::uniform_cylinder;
};

enum class AragatsGeoType {
    uniform,
    pie,
    CORSIKA_USSA
};

enum class ThunderstomPGSubType{
    parma
};

class ParmaSettings{
public:
//    G4String particle = "mu-";
    ParticleField* particle = new ParticleField("mu-");
    G4ThreeVector position = G4ThreeVector();
};




class AragatsSettings {
public:
    double high_boundary = 4250 * meter;
    double low_boundary = 3250 * meter;
    double observed_level = 3200 * meter;
    double pie_step = 50 * meter;
    AragatsGeoType aragatsGeoType = AragatsGeoType::uniform;

    bool only_muon = false;
};


struct TrackingSettings{
    bool saveGamma = false;
    bool saveElectron = false;
    bool savePositron = false;
    bool saveNeutron = false;

};

enum class StackingType{
    simple
};

struct StackingSettings{
    StackingType type = StackingType::simple;
    bool enableElectron = true;
    bool enableGamma = false;
    bool enablePositron = false;
    bool enableMuon = false;
    bool enableNeutron = false;

    bool saveGamma = false;
    bool saveElectron = false;
    double saveElectronCut = 0.0;
    bool savePositron = false;
    bool saveNeutron = false;
};

enum class SteppingType{
    simple,
    critical_energy
};

struct SteppingSettings{
    SteppingType type = SteppingType::simple;


};

class Settings : public ServerSettings {
public:
    double minimal_energy = 0.05 * MeV;

    ThunderstomPGSubType pgSubType = ThunderstomPGSubType::parma;

    string physics = "default";
    string tracking = "default";

    vector<string> particle_cylinder_stacking;
    vector<string> particle_detector;

    GeometrySettings *geometrySettings = new GeometrySettings;
    ParmaSettings* parmaSettings = new ParmaSettings;
    //SteppingAction
    ParticlePredictor *particlePredictor;

    StackingSettings* stackingSettings = new StackingSettings;
    SteppingSettings* steppingSettings = new SteppingSettings;
    TrackingSettings* trackingSettings = new TrackingSettings;

    bool superviseTree = false;

    // Aragats
    AragatsSettings *aragatsSettings = new AragatsSettings;
};


#endif //PHD_CODE_SETTINGS_HH
