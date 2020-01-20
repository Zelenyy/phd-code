//
// Created by mihail on 24.05.17.
//

#ifndef GEANT4_DETECTORCONSTRUCTION_HH
#define GEANT4_DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4GDMLParser.hh"
#include <string>
#include "IFieldFactory.hh"
#include "IDetectorFactory.hh"


using namespace std;

class G4VPhysicalVolume;

class G4LogicalVolume;

class G4Material;

class Logger;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    G4VPhysicalVolume *Construct();

    void ConstructSDandField() override;

    DetectorConstruction(string nameGDML);

    G4GDMLParser *getParser() {
        return &parser;
    }
private:
    string fileName;
    G4GDMLParser parser;
    Logger* logger;
    IFieldFactory* fieldFactory = nullptr;
    IDetectorFactory* detectorFactory = nullptr;
public:
    void setDetectorFactory(IDetectorFactory *detectorFactory);

public:
    void setFieldFactory(IFieldFactory *fieldFactory);

};


#endif //GEANT4_DETECTORCONSTRUCTION_HH
