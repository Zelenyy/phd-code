//
// Created by zelenyy on 05.06.2020.
//

#ifndef PHD_CODE_THUNDERSTORMCONSTRUCTION_HH
#define PHD_CODE_THUNDERSTORMCONSTRUCTION_HH


#include <G4VUserDetectorConstruction.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include "Settings.hh"
#include <G4DormandPrince745.hh>
#include <G4MagIntegratorDriver.hh>
#include <G4ChordFinder.hh>
#include "G4FieldManager.hh"
#include <G4ElectricField.hh>
#include <G4UniformElectricField.hh>
#include <G4EqMagElectricField.hh>

namespace ISA {

    const G4double t0 = 288.15 * kelvin;
    const G4double temperatureGrad = -0.0065 * kelvin / m; //температурный градиент
    const G4double p0 = 101325 * pascal;
    const G4double g = 9.80665 * m / (s * s);
    const G4double M = 0.0289644 * kg / mole;
    const G4double R = 8.31447 * joule / (kelvin * mole);
};

class ThunderstormConstruction : public G4VUserDetectorConstruction {
public:
    explicit ThunderstormConstruction(Settings *settings) : settings(settings) {
        InitializeMaterials();
        logger = Logger::instance();
        geometrySettings = settings->geometrySettings;
    };

protected:
    Settings *settings;
    GeometrySettings* geometrySettings;
    Logger *logger;

    G4double world_radius = 5 * kilometer;
    G4double world_sizeZ = 10 * kilometer;
    G4VPhysicalVolume *constuctWorld();

    G4Material *vacuum;
    G4Material *g4Air;
    G4bool checkOverlaps = true;

    G4Tubs *getCylinder(const string &name, double radius, double heigth);

    void InitializeMaterials() {
        auto nist = G4NistManager::Instance();
        vacuum = nist->FindOrBuildMaterial("G4_Galactic");
        g4Air = nist->FindOrBuildMaterial("G4_AIR");
    };
public:
    static G4double countDensity(G4double height);

    static G4double countPressure(G4double height);

    static G4double countTemperature(G4double height);

    static G4Material *createAirForHeight(G4double height);


};




class AragatsConstruction : public ThunderstormConstruction {

public:

    explicit AragatsConstruction(Settings *settings) : ThunderstormConstruction(settings) {};

    G4VPhysicalVolume *Construct() override;

    void ConstructSDandField() override;

private:

    G4LogicalVolume *detectorLogic = nullptr;
    G4LogicalVolume *cloudLogic = nullptr;

    G4LogicalVolume *CreateUniformCloud();

    G4LogicalVolume *CreateAirPie();

    G4LogicalVolume *CreateCORSIKA();

};


class UniformCylinderConstruction: public ThunderstormConstruction{
private:
    G4LogicalVolume *cloudLogic;
public:
    UniformCylinderConstruction(Settings* settings) : ThunderstormConstruction(settings) {};
    G4VPhysicalVolume *Construct() override{
        world_sizeZ = 2*geometrySettings->cloud_length;
        auto world = constuctWorld();
        auto logicWorld = world->GetLogicalVolume();
        auto air = createAirForHeight(geometrySettings->height);
        double length = geometrySettings->cloud_length;
        auto cloudSolid = getCylinder("cloud", settings->geometrySettings->radius, length);

        cloudLogic = new G4LogicalVolume(cloudSolid, air, "cloud");
        auto cloudPhys = new G4PVPlacement(0, G4ThreeVector(0,0,length/2), cloudLogic, "cloud", logicWorld, false, 0, checkOverlaps);
        return world;
    }
    void ConstructSDandField() override{
        G4VUserDetectorConstruction::ConstructSDandField();
//        auto sdman = G4SDManager::GetSDMpointer();
//        if (detectorLogic != nullptr) {
//            auto particleDetector = new ParticleDetector("detector", settings);
//            sdman->AddNewDetector(particleDetector);
//            detectorLogic->SetSensitiveDetector(particleDetector);
//        }
        logger->print("Set field Z: " + to_string(settings->geometrySettings->field_z / (kilovolt / meter)) + " kV/m");
        G4ElectricField *fEMfield = new G4UniformElectricField(
                G4ThreeVector(0.0, 0.0, -1*settings->geometrySettings->field_z));
        auto *equation = new G4EqMagElectricField(fEMfield);
        G4int nvar = 8;
        auto fStepper = new G4DormandPrince745(equation, nvar);
        auto *fieldManager = new G4FieldManager();
        cloudLogic->SetFieldManager(fieldManager, true);
        fieldManager->SetDetectorField(fEMfield);
        G4double fMinStep;  // minimal step
        fMinStep = 0.01 * mm; // minimal step
        auto integrationDriver = new G4MagInt_Driver(
                fMinStep,
                fStepper,
                fStepper->GetNumberOfVariables()
        );
        auto fChordFinder = new G4ChordFinder(integrationDriver);
        fieldManager->SetChordFinder(fChordFinder);
    };

};


//class CORSIKAUSSA {
//
//    double upper_limit = 112.8*kilometer;
//
//    double T(double height, double a, double b, double c){
//        return a + b*exp(-height/c)
//    }
//
//    G4Material *getMaterial(double height) {
//        double a, b, c, T;
//        if (height < 4 * kilometer) {
//            a =  -186.555305;
//            b =  1222.6562;
//            c = 994186.38*cm;
//
//        } else if (height < 10 * kilometer) {
//            a =  -186.555305;
//            b =  1222.6562;
//            c = 994186.38*cm;
//        } else if (height < 40 * kilometer) {
//            a =  -186.555305;
//            b =  1222.6562;
//            c = 994186.38*cm;
//        } else if (height < 100 * kilometer) {
//            a =  -186.555305;
//            b =  1222.6562;
//            c = 994186.38*cm;
//        } else {
//            a =  -186.555305;
//            b =  1222.6562;
//            c = 994186.38*cm;
//        }
//    }
//};

#endif //PHD_CODE_THUNDERSTORMCONSTRUCTION_HH
