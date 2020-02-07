//
// Created by zelenyy on 03.02.2020.
//

#ifndef PHD_CODE_PARTICLEDETECTOR_HH
#define PHD_CODE_PARTICLEDETECTOR_HH


#include <G4VSensitiveDetector.hh>
#include "Settings.hh"
#include <DataManager.hh>
#include "DataFileManager.hh"
#include "DataFile.hh"
#include <Data.hh>

class ParticleDetector : public G4VSensitiveDetector {
public:
    ParticleDetector(G4String name, Settings *settings);

    G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) override;

private:
    double cut = 0.05 * MeV;
    NamedNumbers *number;
    CylinderData data;
    map<string, DataFile<CylinderData> *> fouts;
};


#endif //PHD_CODE_PARTICLEDETECTOR_HH