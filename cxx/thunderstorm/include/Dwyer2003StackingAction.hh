//
// Created by zelenyy on 31.01.2020.
//

#ifndef PHD_CODE_DWYER2003STACKINGACTION_HH
#define PHD_CODE_DWYER2003STACKINGACTION_HH

#include <G4UserStackingAction.hh>
#include <G4ParticleDefinition.hh>
#include "G4SystemOfUnits.hh"
#include "Settings.hh"
#include "G4Track.hh"
#include "DataFile.hh"
#include <set>
#include "G4StackManager.hh"
#include "map"
#include "ParticlePredictor.hh"
#include "thunderstorm.pb.h"
#include "DataThunderstorm.hh"

class Dwyer2003StackingAction : public G4UserStackingAction {
public:
     explicit Dwyer2003StackingAction(Settings * settings);
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *) override;

    void PrepareNewEvent() override;

private:
    double cut = 0.05*MeV;
    set<int> positronIndx;

    G4ClassificationOfNewTrack ClassifyGamma(const G4Track *);
    ParticlePredictor* fParticlePredictor;

    CylinderId* gammaData;
    CylinderId* positronData;
    DataFileManager* dataFileManager;
};


#endif //PHD_CODE_DWYER2003STACKINGACTION_HH
