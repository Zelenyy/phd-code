//
// Created by mihail on 24.05.17.
//


#include "DetectorConstruction.hh"
#include "G4SDManager.hh"
#include "Logger.hh"
#include <iostream>
#include <G4ElectricField.hh>
#include <G4UniformElectricField.hh>
#include <G4EqMagElectricField.hh>
//#include <G4MagIntegratorStepper.hh>
//#include <G4ClassicalRK4.hh>
#include <G4DormandPrince745.hh>
//#include "G4IntegrationDriver.hh"
#include <G4ChordFinder.hh>
#include "G4FieldManager.hh"
#include "G4ElectricField.hh"
#include <G4MagIntegratorDriver.hh>
#include "G4SystemOfUnits.hh"

using namespace std;


G4VPhysicalVolume *DetectorConstruction::Construct() {
    G4VPhysicalVolume *worldPhys = parser.GetWorldVolume();
    logger->print("World volume " + worldPhys->GetName() + " construct");
    return worldPhys;
}

void DetectorConstruction::ConstructSDandField() {
    const G4GDMLAuxMapType *auxmap = parser.GetAuxMap();
    logger->print("Found " + to_string(auxmap->size()) + " volume(s) with auxiliary information.");

    // The same as above, but now we are looking for
    // sensitive detectors setting them for the volumes

    for (const auto &iter : *auxmap) {
        logger->print("Volume " + (iter.first)->GetName() +
                      " has the following list of auxiliary information: ");
        for (auto vit = iter.second.begin();
             vit != iter.second.end(); vit++) {
            logger->print("--> Type: " + (*vit).type +
                          " Value: " + (*vit).value);
            if ((*vit).type == "SensDet") {
                logger->print("Attaching sensitive detector " + (*vit).value
                              + " to volume " + (iter.first)->GetName());

                G4VSensitiveDetector *mydet = detectorFactory->getSensitiveDetector(vit);
                if (mydet) {
                    G4LogicalVolume *myvol = iter.first;
                    myvol->SetSensitiveDetector(mydet);
                } else {
                    logger->print((*vit).value + " detector not found");
                }
            } else if ((*vit).type == "ElectricField") {
                logger->print("Construct field: " + (*vit).value + " in " + (iter.first)->GetName());
                G4ElectricField *fEMfield = fieldFactory->getElectricFieldFromGDMLAux(vit);
                auto *equation = new G4EqMagElectricField(fEMfield);
                G4int nvar = 8;
                auto fStepper = new G4DormandPrince745(equation, nvar);
                auto *fieldManager = new G4FieldManager();
                auto fieldVol = iter.first;
                fieldVol->SetFieldManager(fieldManager, true);
                fieldManager->SetDetectorField(fEMfield);
                G4double fMinStep;  // minimal step
                fMinStep = 0.01 * mm; // minimal step
//                auto integrationDriver = new G4IntegrationDriver<G4DormandPrince745>(
//                        fMinStep,
//                        fStepper,
//                        nvar);
                auto integrationDriver = new G4MagInt_Driver(
                        fMinStep,
                        fStepper,
                        fStepper->GetNumberOfVariables()
                        );
                auto fChordFinder = new G4ChordFinder(integrationDriver);
                fieldManager->SetChordFinder(fChordFinder);
            }


        }
    }
}


DetectorConstruction::DetectorConstruction(string nameGDML) : G4VUserDetectorConstruction(), fileName(nameGDML) {
    parser.Read(fileName, false);
    logger = Logger::instance();
    logger->print("GDML file " + fileName + " read");

    auto auxList = parser.GetAuxList();
    for (const auto &it : *auxList) {
        logger->print(it.type + " " + it.value + " " + it.unit);
    }


}

void DetectorConstruction::setFieldFactory(IFieldFactory *fieldFactory_) {
    fieldFactory = fieldFactory_;
}

void DetectorConstruction::setDetectorFactory(IDetectorFactory *detectorFactory_) {
    detectorFactory = detectorFactory_;
}
