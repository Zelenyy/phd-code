#include <utility>

//
// Created by zelenyy on 12.02.20.
//

#ifndef PHD_CODE_PARTICLEPREDICTOR_HH
#define PHD_CODE_PARTICLEPREDICTOR_HH

#include <G4ClassificationOfNewTrack.hh>
#include <G4Track.hh>
#include <random>
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include "Histogramm.hh"
#include "G4SystemOfUnits.hh"
using namespace std;

using namespace CLHEP;

class ParticlePredictor {

public:
//    Histogramm3D* fHistogramm3D;
    Histogramm2D* fHist2DLow;
    Histogramm2D* fHist2DHight;

    Histogramm2D* fGammaHist2DLow;
    Histogramm2D* fGammaHist2DHight;


    ParticlePredictor(){
        Bins* energyLow = new UniformBins(0.05, 1, 19);
        Bins* energyHight = new UniformBins(1, 101, 25);
//        Bins* energyHightAccuracy = new UniformBins(1, 100, 4);

        Bins* z = new UniformBins(-200, 200, 40);
//        Bins* theta = new
//        fHistogramm3D = new Histogramm3D();
        fHist2DLow = new Histogramm2D(energyLow, z);
        fHist2DHight = new Histogramm2D(energyHight, z);

        fGammaHist2DLow = new Histogramm2D(energyLow, z);
        fGammaHist2DHight = new Histogramm2D(energyHight, z);

        unif = new uniform_real_distribution<double>(0,1);
    }
    bool accept(const G4Track* aTrack){
        if (aTrack->GetDefinition()==G4Gamma::Definition()){
            if (aTrack->GetKineticEnergy() < 1){
                fGammaHist2DLow->add(aTrack->GetKineticEnergy(), aTrack->GetPosition().getZ() / meter);
                double sample = (*unif)(re);
                return sample > 0.8;
            }
            else{
                fGammaHist2DHight->add(aTrack->GetKineticEnergy(), aTrack->GetPosition().getZ() / meter);
            }

            return true;

        }
        if (aTrack->GetDefinition() == G4Electron::Definition()){
            if (aTrack->GetKineticEnergy() < 1){
                fHist2DLow->add(aTrack->GetKineticEnergy(), aTrack->GetPosition().getZ() /meter);
                double sample = (*unif)(re);
                return sample > 0.8;
            }
            else{
                fHist2DHight->add(aTrack->GetKineticEnergy(), aTrack->GetPosition().getZ() / meter);
            }

            return true;

        }
        return false;

    };
private:
    default_random_engine re;
    uniform_real_distribution<double>* unif;

};


#endif //PHD_CODE_PARTICLEPREDICTOR_HH
