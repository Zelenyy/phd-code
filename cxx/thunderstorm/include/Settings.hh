//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_SETTINGS_HH
#define PHD_CODE_SETTINGS_HH

#include "G4SystemOfUnits.hh"
#include "ParticlePredictor.hh"
using namespace std;

struct Settings{
    string physics = "default";
    string stacking = "default";
    string stepping = "default";
    string tracking = "default";
    double born_cut = 0.05*MeV;
    vector<string> particle_cylinder_stacking;
    vector<string> particle_detector;

    //SteppingAction
    bool stepping_energy_cut = true;
    ParticlePredictor* particlePredictor;
};

#endif //PHD_CODE_SETTINGS_HH
