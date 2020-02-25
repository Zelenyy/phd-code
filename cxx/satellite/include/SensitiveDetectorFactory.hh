//
// Created by zelenyy on 07.12.17.
//

#ifndef GEANT4_SATELLITE_SENSITIVEDETECTORFACTORY_HH
#define GEANT4_SATELLITE_SENSITIVEDETECTORFACTORY_HH


#include "G4VSensitiveDetector.hh"
#include "string"
#include "IDetectorFactory.hh"
#include "G4GDMLParser.hh"
#include "Settings.hh"

using namespace std;

class SensitiveDetectorFactory : public IDetectorFactory {
public:
    explicit SensitiveDetectorFactory(Settings* settings): fSettings(settings){};

    G4VSensitiveDetector *getSensitiveDetector(G4GDMLAuxListType::const_iterator vit) override;

private:
    Settings* fSettings;
};


#endif //GEANT4_SATELLITE_SENSITIVEDETECTORFACTORY_HH
