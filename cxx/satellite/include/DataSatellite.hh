//
// Created by zelenyy on 22.02.2020.
//

#ifndef PHD_CODE_DATASATELLITE_HH
#define PHD_CODE_DATASATELLITE_HH

#include "satellite.pb.h"
#include "Settings.hh"

class DataSatellite {
public:
    satellite::Run* run = nullptr;
    satellite::MeanRun* meanRun = nullptr;

    satellite::Run* wrapperRun = nullptr;

    void initialize(Settings* settings){
        if (settings->scoredDetectorMode == ScoredDetectorMode::single){
            run = new satellite::Run();
        }
        else{
            meanRun = new satellite::MeanRun();
        }
    }

    static DataSatellite *instance() {
        static DataSatellite dataSatellite;

        return &dataSatellite;
    }
private:
    DataSatellite()= default;

    DataSatellite(DataSatellite const &) = delete;

    DataSatellite &operator=(DataSatellite const &) = delete;
};


#endif //PHD_CODE_DATASATELLITE_HH
