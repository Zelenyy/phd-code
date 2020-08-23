//
// Created by zelenyy on 03.02.2020.
//

#ifndef PHD_CODE_STACKINGACTION_HH
#define PHD_CODE_STACKINGACTION_HH


#include <G4UserStackingAction.hh>
#include <G4ParticleDefinition.hh>
#include "G4SystemOfUnits.hh"
#include "Settings.hh"
#include "G4Track.hh"
#include "DataFile.hh"

class StackingAction : public G4UserStackingAction{
public:
    explicit StackingAction(Settings* settings);
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *) override;
    void PrepareNewEvent() override;

private:
    Settings* fSettings;
    StackingSettings * stackingSettings;
    double cut = 0.05*MeV;
    bool only_muon = false;

    G4ClassificationOfNewTrack ClassifyGamma(const G4Track * aTrack);
    G4ClassificationOfNewTrack ClassifyElectron(const G4Track * aTrack);
    G4ClassificationOfNewTrack ClassifyPositron(const G4Track * aTrack);
    G4ClassificationOfNewTrack ClassifyMuon(const G4Track * aTrack);


};


#endif //PHD_CODE_STACKINGACTION_HH
