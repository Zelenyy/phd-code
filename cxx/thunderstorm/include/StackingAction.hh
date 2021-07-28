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
#include "DataThunderstorm.hh"

class StackingAction : public G4UserStackingAction{
public:
    explicit StackingAction(Settings* settings);
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *) override;
    void PrepareNewEvent() override;

private:
    Settings* fSettings;
    StackingSettings * stackingSettings;
    SuperviseTree* superviseTree;
    double cut = 0.05*MeV;

    CylinderId* data = nullptr;

    G4ClassificationOfNewTrack ClassifyGamma(const G4Track * aTrack);
    G4ClassificationOfNewTrack ClassifyElectron(const G4Track * aTrack);
    G4ClassificationOfNewTrack ClassifyPositron(const G4Track * aTrack);
    G4ClassificationOfNewTrack ClassifyMuon(const G4Track * aTrack);


    G4ClassificationOfNewTrack ClassifyNeutron(const G4Track *pTrack);
};


#endif //PHD_CODE_STACKINGACTION_HH
