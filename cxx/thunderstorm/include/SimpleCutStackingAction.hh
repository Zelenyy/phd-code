//
// Created by zelenyy on 03.02.2020.
//

#ifndef PHD_CODE_SIMPLECUTSTACKINGACTION_HH
#define PHD_CODE_SIMPLECUTSTACKINGACTION_HH


#include <G4UserStackingAction.hh>
#include <G4UserStackingAction.hh>
#include <G4ParticleDefinition.hh>
#include "G4SystemOfUnits.hh"
#include "Settings.hh"
#include "G4Track.hh"
#include "DataFile.hh"

class SimpleCutStackingAction : public G4UserStackingAction{
public:
    explicit SimpleCutStackingAction(Settings* settings);
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *) override;
    void PrepareNewEvent() override;

private:
    double cut = 0.05*MeV;
    bool only_muon = false;
};


#endif //PHD_CODE_SIMPLECUTSTACKINGACTION_HH
