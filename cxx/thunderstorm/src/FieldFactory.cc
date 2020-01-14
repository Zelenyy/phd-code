//
// Created by zelenyy on 04.11.17.
//

#include "FieldFactory.hh"

using namespace std;

G4ElectricField *FieldFactory::getElectricFieldFromGDMLAux(G4GDMLAuxListType::const_iterator vit) {

        return IFieldFactory::getElectricFieldFromGDMLAux(vit);
}
