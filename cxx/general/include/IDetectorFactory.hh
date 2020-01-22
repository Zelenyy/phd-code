//
// Created by zelenyy on 17.02.18.
//

#ifndef GEANT4_SUPPORT_CXX_IDETECTORFACTORY_HH
#define GEANT4_SUPPORT_CXX_IDETECTORFACTORY_HH

#include "G4VSensitiveDetector.hh"
#include "string"
#include "G4GDMLParser.hh"
#include "Logger.hh"

using namespace std;

class IDetectorFactory {
public:
    virtual G4VSensitiveDetector *getSensitiveDetector(G4GDMLAuxListType::const_iterator vit) {
        auto name = (*vit).value;
        Logger::instance()->print("Invalid detector name: " + name);
        return nullptr;
    };
};


#endif //GEANT4_SUPPORT_CXX_IDETECTORFACTORY_HH
