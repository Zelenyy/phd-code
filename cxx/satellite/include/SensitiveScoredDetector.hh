//
// Created by zelenyy on 18.10.17.
//

#ifndef GEANT4_THUNDERSTORM_SENSITIVESCOREDDETECTOR_HH
#define GEANT4_THUNDERSTORM_SENSITIVESCOREDDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "iostream"
#include "fstream"
#include "DataFileManager.hh"
#include "DataFile.hh"

#include "vector"
#include "Settings.hh"
#include "DataSatellite.hh"
#include "DataStorage.hh"
using namespace CLHEP;
using namespace std;

class G4Step;

class G4HCofThisEvent;

class G4TouchableHistory;



class SensitiveScoredDetector : public G4VSensitiveDetector {
public:
    SensitiveScoredDetector(G4String name, Settings *settings);

    void Initialize(G4HCofThisEvent *) override;

    G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) override;

    void EndOfEvent(G4HCofThisEvent *) override;

private:
    EnergyDepositData data{};
    Settings* fSettings;

    MonolithDataCell<EnergyDepositData,1000>* dataCell;

    DataFile<EnergyDepositData>* foutDeposit;
    SocketOutput<EnergyDepositData>* socketOutput;
};


#endif //GEANT4_THUNDERSTORM_SENSITIVESCOREDDETECTOR_HH
