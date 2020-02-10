//
// Created by zelenyy on 31.01.2020.
//

#ifndef PHD_CODE_DWYER2003STACKINGACTION_HH
#define PHD_CODE_DWYER2003STACKINGACTION_HH

#include <G4UserStackingAction.hh>
#include <G4ParticleDefinition.hh>
#include "G4SystemOfUnits.hh"
#include "Settings.hh"
#include "Data.hh"
#include "G4Track.hh"
#include "DataFile.hh"
#include <DataManager.hh>
#include "G4StackManager.hh"

class Dwyer2003StackingAction : public G4UserStackingAction {
public:
     explicit Dwyer2003StackingAction(Settings * settings);
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *) override;

    void PrepareNewEvent() override;

private:
    double cut = 0.05*MeV;
    vector<int> parent;
    Numbers* number;
    CylinderIdData data;
    DataFile<CylinderIdData> *foutGamma;
    DataFile<CylinderIdData> *foutPositron;

    G4ClassificationOfNewTrack ClassifyGamma(const G4Track *);
};


#endif //PHD_CODE_DWYER2003STACKINGACTION_HH
