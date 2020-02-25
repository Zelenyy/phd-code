//
// Created by zelenyy on 07.12.17.
//

#include "SensitiveDetectorFactory.hh"
#include "SensitiveScoredDetector.hh"
#include "G4SDManager.hh"

using namespace std;


G4VSensitiveDetector *SensitiveDetectorFactory::getSensitiveDetector(G4GDMLAuxListType::const_iterator vit) {
    auto name = (*vit).value;
    G4SDManager *fSDM = G4SDManager::GetSDMpointer();
    G4VSensitiveDetector *tempDetector;
    if (name == "SensitiveScoredDetector") {
        tempDetector = new SensitiveScoredDetector(name, fSettings);
    } else {
        tempDetector = IDetectorFactory::getSensitiveDetector(vit);
    }


    fSDM->AddNewDetector(tempDetector);
    G4cout << "Create new detector: " << name << endl;
    return tempDetector;
}
