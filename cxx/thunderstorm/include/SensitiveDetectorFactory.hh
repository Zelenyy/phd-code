//
// Created by zelenyy on 18.10.17.
//

#ifndef GEANT4_THUNDERSTORM_SENSITIVEDETECTORFACTORY_HH
#define GEANT4_THUNDERSTORM_SENSITIVEDETECTORFACTORY_HH

#include "G4VSensitiveDetector.hh"
#include "string"
#include "IDetectorFactory.hh"
using namespace std;
class SensitiveDetectorFactory: public IDetectorFactory {
public:
    explicit SensitiveDetectorFactory(Settings* settings);
    G4VSensitiveDetector *getSensitiveDetector(G4GDMLAuxListType::const_iterator vit) override;

private:
    Settings* settings;
};


#endif //GEANT4_THUNDERSTORM_SENSITIVEDETECTORFACTORY_HH
