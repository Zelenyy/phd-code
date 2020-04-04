//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_ONEGENERATIONSTACKINGACTION_HH
#define PHD_CODE_ONEGENERATIONSTACKINGACTION_HH


#include <G4UserStackingAction.hh>
#include <G4ParticleDefinition.hh>
#include "G4SystemOfUnits.hh"
#include "Settings.hh"
#include "G4Track.hh"
#include "DataFile.hh"

using namespace std;

class OneGenerationStackingAction : public G4UserStackingAction {
public:
    explicit OneGenerationStackingAction(Settings * settings);
    ~OneGenerationStackingAction();
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *) override;

    void PrepareNewEvent() override;

private:
    double cut = 0.05*MeV;
    G4ParticleDefinition* primaryParticle;
    bool flag = false;
};


#endif //PHD_CODE_ONEGENERATIONSTACKINGACTION_HH
