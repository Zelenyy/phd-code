//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_SETTINGS_HH
#define PHD_CODE_SETTINGS_HH

#include "G4SystemOfUnits.hh"
#include "ServerSettings.hh"
#include <G4Server.hh>

using namespace std;


enum class SatelliteSubType {
    tyvek
};

struct GeometrySettings {
//    double height = 0.0 * meter;
//    double field_z = 0.0 * kilovolt / meter;
//    double radius = 4 * kilometer;
//    double cloud_length = 1000*meter;
    SatelliteSubType geometryType = SatelliteSubType::tyvek;
};

enum class ScoredDetectorMode{
    sum,
    single
};

enum class OutputMode{
    file,
    socket_client
};

class Settings : public ServerSettings {
public:
    int number_of_cell = 100;
    ScoredDetectorMode scoredDetectorMode = ScoredDetectorMode::sum;
    OutputMode outputMode =OutputMode::file;
    int port = 8777;
    GeometrySettings *geometrySettings = new GeometrySettings;
};

#endif //PHD_CODE_SETTINGS_HH
