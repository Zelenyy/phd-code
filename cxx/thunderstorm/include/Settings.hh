//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_SETTINGS_HH
#define PHD_CODE_SETTINGS_HH

#include "G4SystemOfUnits.hh"

using namespace std;

struct Settings{
    string physics = "default";
    string stacking = "default";
    double born_cut = 0.05*MeV;
};

#endif //PHD_CODE_SETTINGS_HH
