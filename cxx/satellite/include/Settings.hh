//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_SETTINGS_HH
#define PHD_CODE_SETTINGS_HH

#include "G4SystemOfUnits.hh"

using namespace std;

enum class ScoredDetectorMode{
    sum,
    single
};

enum class OutputMode{
    file,
    socket_client
};

struct Settings{
    int number_of_cell = 100;
    ScoredDetectorMode scoredDetectorMode = ScoredDetectorMode::sum;
    OutputMode outputMode =OutputMode::file;
    int port = 8777;
};

#endif //PHD_CODE_SETTINGS_HH
