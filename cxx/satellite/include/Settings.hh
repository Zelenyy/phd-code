//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_SETTINGS_HH
#define PHD_CODE_SETTINGS_HH

#include "G4SystemOfUnits.hh"

using namespace std;

enum class ScoredDetectorMode{
    mean,
    single
};

enum class OutputMode{
    file,
    socket_client
};

struct Settings{
    int number_of_cell = 100;
    ScoredDetectorMode scoredDetectorMode = ScoredDetectorMode::mean;
    OutputMode outputMode =OutputMode::file;
};

#endif //PHD_CODE_SETTINGS_HH
