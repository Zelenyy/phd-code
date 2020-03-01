//
// Created by zelenyy on 22.02.2020.
//

#ifndef PHD_CODE_DATASATELLITE_HH
#define PHD_CODE_DATASATELLITE_HH

#include "satellite.pb.h"

class DataSatellite {
public:
    satellite::Run* run;
    static DataSatellite *instance() {
        static DataSatellite dataSatellite;

        return &dataSatellite;
    }
private:
    DataSatellite(){
        run = new satellite::Run();
    };

    DataSatellite(DataSatellite const &) = delete;

    DataSatellite &operator=(DataSatellite const &) = delete;
};


#endif //PHD_CODE_DATASATELLITE_HH
