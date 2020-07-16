//
// Created by zelenyy on 26.06.2020.
//

#ifndef PHD_CODE_PARMA_HH
#define PHD_CODE_PARMA_HH

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <random>
#include <G4ThreeVector.hh>

struct PARMAEvent{
    double Energy;
    CLHEP::Hep3Vector direction;
    CLHEP::Hep3Vector position;
};

class PARMAGenerator {
public:
    PARMAGenerator();

    PARMAEvent generate();

private:
    // Particle Generation
    std::mt19937 engine; // Mersenne Twister
    std::uniform_real_distribution<double> rand01 = std::uniform_real_distribution<double>(0.0, 1.0); // random number between 0 to 1
private:
    static const int npart = 33;
    int IangPart[npart+1] = {1,2,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,5,5,6};
    static const int nebin=100; // number of energy mesh (divided by log)
    static const int nabin=100;  // number of angle mesh (divided by linear)
    double ehigh[nebin+1],emid[nebin+1]; // higher and middle point of energy bin
    double ahigh[nabin+1],amid[nabin+1]; // higher and middle point of angular bin
    double etable[nebin+1] = {}; // probability table (0.0 for 0, 1.0 for nebin)
    double atable[nabin+1][nebin+1] = {}; // probability table (0.0 for 0, 1.0 for nabin)
    double atable2[nabin+1]; // temporary used dimension for anguluar probability
    double Flux[nabin+1][nebin+1] = {}; // Monte Carlo generated flux

    int ia,ie,i,ia2;

    double TotalFlux;

public:
    // Set condition
//    int nevent=1000; // number of particles to be generated
    int ip = 1; // Particle ID (Particle ID, 0:neutron, 1-28:H-Ni, 29-30:muon+-, 31:e-, 32:e+, 33:photon)
    int iyear = 2019; // Year
    int imonth = 2;   // Month
    int iday = 1;    // Day
    double glat = 30.5; // Latitude (deg), -90 =< glat =< 90
    double glong = -76.2; // Longitude (deg), -180 =< glong =< 180
    double alti = 0.0; // Altitude (km)
    double g = 0.15; // Local geometry parameter, 0=< g =< 1: water weight fraction, 10:no-earth, 100:blackhole, -10< g < 0: pilot, g < -10: cabin
    double radi = 100.0; // radius of the target area (put your target inside this area)

// Set energy and angle ranges for generation
    double emin = 1.0e0;  // Minimum energy of particle
    double emax = 1.0e5;  // Maximum energy of particle
    double amin = -1.0;   // Minimum cosine of particle
    double amax =  1.0;   // Maximum cosine of particle
};


#endif //PHD_CODE_PARMA_HH
