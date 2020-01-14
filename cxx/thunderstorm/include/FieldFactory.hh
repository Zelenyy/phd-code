//
// Created by zelenyy on 04.11.17.
//

#ifndef GEANT4_TEMPLATE_CXX_FIELDFACTORY_HH
#define GEANT4_TEMPLATE_CXX_FIELDFACTORY_HH

#include "G4ElectricField.hh"
#include "G4GDMLParser.hh"
#include "IFieldFactory.hh"


class FieldFactory: public IFieldFactory {
public:
    G4ElectricField* getElectricFieldFromGDMLAux(G4GDMLAuxListType::const_iterator vit);
};


#endif //GEANT4_TEMPLATE_CXX_FIELDFACTORY_HH
