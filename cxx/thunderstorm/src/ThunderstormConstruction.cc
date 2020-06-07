//
// Created by zelenyy on 05.06.2020.
//

#include "ThunderstormConstruction.hh"
#include <G4SDManager.hh>
#include <ParticleDetector.hh>
#include <G4DormandPrince745.hh>
#include <G4MagIntegratorDriver.hh>
#include <G4ChordFinder.hh>
#include "G4FieldManager.hh"
#include <G4ElectricField.hh>
#include <G4UniformElectricField.hh>
#include <G4EqMagElectricField.hh>

G4VPhysicalVolume *ThunderstormConstruction::constuctWorld() {

    G4double world_radius = 4.5 * kilometer;
    G4double world_sizeZ = 1.5 * kilometer;

    G4VSolid* solidWorld =
            getCylinder("World",
                       world_radius,
                       world_sizeZ);     //its size

    G4LogicalVolume* logicWorld =
            new G4LogicalVolume(solidWorld,          //its solid
                                vacuum,           //its material
                                "World");            //its name

    G4VPhysicalVolume* physWorld =
            new G4PVPlacement(0,                     //no rotation
                              G4ThreeVector(),       //at (0,0,0)
                              logicWorld,            //its logical volume
                              "World",               //its name
                              0,                     //its mother  volume
                              false,                 //no boolean operation
                              0,                     //copy number
                              checkOverlaps);
    return physWorld;
}

G4Tubs* ThunderstormConstruction::getCylinder(const string& name, double radius, double heigth) {
    return new G4Tubs(G4String(name), 0, radius, 0.5*heigth, 0, twopi);
}

G4double ThunderstormConstruction::countPressure(G4double height) {
    G4double temperature = countTemperature(height);
    G4double pressure = ISA::p0*exp(-1*ISA::M*ISA::g*height/(ISA::R*temperature));
    return pressure;
}

G4double ThunderstormConstruction::countDensity(G4double height) {
    G4double temperature = countTemperature(height);
    G4double pressure = countPressure(height);
    G4double density = pressure*ISA::M/(ISA::R*temperature);
    return density;
}

G4double ThunderstormConstruction::countTemperature(G4double height) {
        G4double temperature = ISA::t0 + ISA::temperatureGrad * height;
        return temperature;
}

G4Material *ThunderstormConstruction::createAirForHeight(G4double height) {
    G4NistManager* nistManager = G4NistManager::Instance();
    G4Material* air = nistManager->FindOrBuildMaterial("G4_AIR");
    const G4double density = countDensity(height);
    const G4double pressure = countPressure(height);
    const G4double temperature = countTemperature(height);
    string name = "_" + to_string((int) ceil(height/meter));
    Logger::instance()->print("airHeight"+name);
    Logger::instance()->print("Density: " + to_string(density/(kg/m3)) + " kg/m3");
    Logger::instance()->print("Pressure: " + to_string(pressure/pascal) + " pascal");
    Logger::instance()->print("Temperature: " + to_string(temperature/kelvin) + " kelvin");
    G4Material* airHeight = new G4Material("airHeight"+name, density, air,
                                           kStateGas,temperature,pressure);
    return airHeight;
}

G4VPhysicalVolume *AragatsConstruction::Construct() {
    auto world = constuctWorld();
    auto logicWorld = world->GetLogicalVolume();

    auto aragats_air_mean = createAirForHeight(settings->geometrySettings->height);

    auto cloudSolid = getCylinder("cloud", 4*kilometer, 1*kilometer);

    cloudLogic = new G4LogicalVolume(cloudSolid, aragats_air_mean, "cloud");
    auto cloudPhys = new G4PVPlacement(0, G4ThreeVector(), cloudLogic, "cloud", logicWorld, false, 0, checkOverlaps);


    auto air_3200 = createAirForHeight(3200*meter);
    auto air_3225 = createAirForHeight(3325*meter);

    auto noFieldSolid = getCylinder("noField", 4*kilometer, 50*meter);
    auto detectorSolid = getCylinder("detector", 4*kilometer, 10*meter);
    detectorLogic = new G4LogicalVolume(detectorSolid, air_3200, "detector");
    double cloud_size = 500*meter;
    double no_field_position = -25*meter - cloud_size;
    double detector_position = -5.0*meter -25*meter + no_field_position;
//    double detector_pos = -((aragats::high_boundary - aragats::low_boundary)/2) - (aragats::low_boundary - aragats::observed_level)
    auto detectorPhys = new  G4PVPlacement(0, G4ThreeVector(0.0, 0.0, detector_position), detectorLogic, "detector", logicWorld, false, 0, checkOverlaps);
    auto nofieldPhys = new  G4PVPlacement(0, G4ThreeVector(0.0, 0.0, no_field_position), detectorLogic, "noField", logicWorld, false, 0, checkOverlaps);

    return world;
}

void AragatsConstruction::ConstructSDandField() {
    G4VUserDetectorConstruction::ConstructSDandField();
    auto sdman = G4SDManager::GetSDMpointer();
    if (detectorLogic != nullptr){
        auto particleDetector = new ParticleDetector("detector", settings);
        sdman->AddNewDetector(particleDetector);
        detectorLogic->SetSensitiveDetector(particleDetector);
    }
    logger->print("Set field Z: " + to_string(settings->geometrySettings->field_z/(kilovolt/meter)) + " kV/m");
    G4ElectricField *fEMfield = new G4UniformElectricField(G4ThreeVector(0.0, 0.0, settings->geometrySettings->field_z));
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
}
