//
// Created by zelenyy on 26.06.2020.
//

#ifndef PHD_CODE_PARMA_HH
#define PHD_CODE_PARMA_HH


#include <G4ThreeVector.hh>

struct PARMAEvent{
    double Energy;
    CLHEP::Hep3Vector direction;
    CLHEP::Hep3Vector position;
};

class PARMAGenerator {

    PARMAGenerator();

    PARMAEvent generate();
};


#endif //PHD_CODE_PARMA_HH
