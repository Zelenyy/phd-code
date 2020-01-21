//
// Created by zelenyy on 17.02.18.
//

#ifndef GEANT4_SUPPORT_CXX_IFIELDFACTORY_HH
#define GEANT4_SUPPORT_CXX_IFIELDFACTORY_HH

#include "G4UniformElectricField.hh"
#include "G4ElectricField.hh"
#include "G4GDMLParser.hh"
#include "string"
#include "map"
#include "Logger.hh"
#include "G4SystemOfUnits.hh"
using namespace std;
using  namespace CLHEP;

class IFieldFactory {
public:
    virtual G4ElectricField *getElectricFieldFromGDMLAux(G4GDMLAuxListType::const_iterator vit) {
        auto logger = Logger::instance();
        logger->print("Start Ifield factory");
        if (vit->value == "Uniform") {
            auto fieldList = (*vit).auxList;
            map<string, double> E;
            for (auto & fieldIt : *fieldList) {
                E[fieldIt.type] = stod(fieldIt.value);
                string ESi = to_string(E[fieldIt.type] / (kilovolt/meter));
                Logger::instance()->print(fieldIt.type + " = " + ESi + " kilovolt/meter");
            }
            G4ElectricField *fEMfield = new G4UniformElectricField(G4ThreeVector(E["Ex"], E["Ey"], E["Ez"]));
            logger->print("IFieldFactory: \"I produce field: Uniform\"");
            return fEMfield;

        }
        return nullptr;
    };
};


#endif //GEANT4_SUPPORT_CXX_IFIELDFACTORY_HH
