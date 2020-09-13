//
// Created by zelenyy on 22.08.2020.
//

#ifndef PHD_CODE_SERVERSETTINGS_HH
#define PHD_CODE_SERVERSETTINGS_HH

#include "string"

using namespace std;

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

#endif //PHD_CODE_SERVERSETTINGS_HH
