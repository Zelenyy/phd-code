//
// Created by zelenyy on 03.02.2020.
//

#ifndef PHD_CODE_PARTICLECYLINDERSTACKING_HH
#define PHD_CODE_PARTICLECYLINDERSTACKING_HH
#include <G4UserStackingAction.hh>
#include <G4UserStackingAction.hh>
#include <G4ParticleDefinition.hh>
#include "G4SystemOfUnits.hh"
#include "Settings.hh"
#include "Data.hh"
#include "G4Track.hh"
#include "DataFile.hh"
#include <DataManager.hh>

class ParticleCylinderStacking : public G4UserStackingAction {
public:
    explicit ParticleCylinderStacking(Settings* settings);
    G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track *) override;
    void PrepareNewEvent() override;

private:
    double cut = 0.05*MeV;
    Settings* settings;
    NamedNumbers *number;
    CylinderData data;
    map<string, DataFile<CylinderData> *> fouts;
};


#endif //PHD_CODE_PARTICLECYLINDERSTACKING_HH
