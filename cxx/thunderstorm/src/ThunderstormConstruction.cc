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
    G4double world_sizeZ = 4.5 * kilometer;

    G4VSolid *solidWorld =
            getCylinder("World",
                        world_radius,
                        world_sizeZ);     //its size

    G4LogicalVolume *logicWorld =
            new G4LogicalVolume(solidWorld,          //its solid
                                vacuum,           //its material
                                "World");            //its name

    G4VPhysicalVolume *physWorld =
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

G4Tubs *ThunderstormConstruction::getCylinder(const string &name, double radius, double heigth) {
    return new G4Tubs(G4String(name), 0, radius, 0.5 * heigth, 0, twopi);
}

G4double ThunderstormConstruction::countPressure(G4double height) {
    G4double temperature = countTemperature(height);
    G4double pressure = ISA::p0 * exp(-1 * ISA::M * ISA::g * height / (ISA::R * temperature));
    return pressure;
}

G4double ThunderstormConstruction::countDensity(G4double height) {
    G4double temperature = countTemperature(height);
    G4double pressure = countPressure(height);
    G4double density = pressure * ISA::M / (ISA::R * temperature);
    return density;
}

G4double ThunderstormConstruction::countTemperature(G4double height) {
    G4double temperature = ISA::t0 + ISA::temperatureGrad * height;
    return temperature;
}

G4Material *ThunderstormConstruction::createAirForHeight(G4double height) {
    // TODO Add corisika
    G4NistManager *nistManager = G4NistManager::Instance();
    G4Material *air = nistManager->FindOrBuildMaterial("G4_AIR");
    const G4double density = countDensity(height);
    const G4double pressure = countPressure(height);
    const G4double temperature = countTemperature(height);
    string name = "_" + to_string((int) ceil(height / meter));
    Logger::instance()->print("airHeight" + name);
    Logger::instance()->print("Density: " + to_string(density / (kg / m3)) + " kg/m3");
    Logger::instance()->print("Pressure: " + to_string(pressure / pascal) + " pascal");
    Logger::instance()->print("Temperature: " + to_string(temperature / kelvin) + " kelvin");
    G4Material *airHeight = new G4Material("airHeight" + name, density, air,
                                           kStateGas, temperature, pressure);
    return airHeight;
}

G4VPhysicalVolume *AragatsConstruction::Construct() {
    auto world = constuctWorld();
    auto logicWorld = world->GetLogicalVolume();


    if (settings->aragatsSettings->aragatsGeoType == AragatsGeoType::uniform) {
        cloudLogic = CreateUniformCloud();
    } else if (settings->aragatsSettings->aragatsGeoType == AragatsGeoType::pie) {
        cloudLogic = CreateAirPie();
    } else if (settings->aragatsSettings->aragatsGeoType == AragatsGeoType::CORSIKA_USSA){

    }

    auto cloudPhys = new G4PVPlacement(0, G4ThreeVector(), cloudLogic, "cloud", logicWorld, false, 0, checkOverlaps);

    double radius = settings->geometrySettings->radius;
    double h_obs_lvl = settings->aragatsSettings->observed_level;
    auto air_obs_lvl = createAirForHeight(h_obs_lvl);
    double h_low = settings->aragatsSettings->low_boundary;
    double h_high = settings->aragatsSettings->high_boundary;
    auto air_no_field = createAirForHeight(h_obs_lvl + (h_low - h_obs_lvl) / 2);

    double cloud_size = (h_high - h_low) / 2;
    double no_field_position = -(h_low - h_obs_lvl) / 2 - cloud_size;

    if (h_low - h_obs_lvl > 0.0){
        auto noFieldSolid = getCylinder("noField", radius, h_low - h_obs_lvl);
        auto noFieldLogic = new G4LogicalVolume(noFieldSolid, air_no_field, "noField");
        auto nofieldPhys = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, no_field_position), noFieldLogic, "noField",
                                             logicWorld, false, 0, checkOverlaps);
    }

    auto detectorSolid = getCylinder("detector", radius, 10 * meter);
    detectorLogic = new G4LogicalVolume(detectorSolid, air_obs_lvl, "detector");
    double detector_position = -5.0 * meter - (h_low - h_obs_lvl) / 2 + no_field_position;
    auto detectorPhys = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, detector_position), detectorLogic, "detector",
                                          logicWorld, false, 0, checkOverlaps);
    return world;
}

void AragatsConstruction::ConstructSDandField() {
    G4VUserDetectorConstruction::ConstructSDandField();
    auto sdman = G4SDManager::GetSDMpointer();
    if (detectorLogic != nullptr) {
        auto particleDetector = new ParticleDetector("detector", settings);
        sdman->AddNewDetector(particleDetector);
        detectorLogic->SetSensitiveDetector(particleDetector);
    }
    logger->print("Set field Z: " + to_string(settings->geometrySettings->field_z / (kilovolt / meter)) + " kV/m");
    G4ElectricField *fEMfield = new G4UniformElectricField(
            G4ThreeVector(0.0, 0.0, settings->geometrySettings->field_z));
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

G4LogicalVolume *AragatsConstruction::CreateAirPie() {
    double start = settings->aragatsSettings->low_boundary;
    double end = settings->aragatsSettings->high_boundary;
    double step = settings->aragatsSettings->pie_step;
    double positionShift = -1 * (start + end) / 2;
    double height;
    double thinkness, halfThinkness;
    G4Material *mat;
    G4Tubs *cellSolid;
    G4LogicalVolume *cellLogic;
    G4VPhysicalVolume *cell;
    G4ThreeVector position;
    string name;

    auto cloudSolid = getCylinder("cloud_pie", settings->geometrySettings->radius, end-start);
    auto cloudLogic = new G4LogicalVolume(cloudSolid, vacuum, "cloud_pie");

    int indx = 0;
    for (double i = start; i < end; i += step) {
        if (i + step <= end) {
            height = i + step / 2;
            thinkness = step;

        } else {
            height = i + (end - i) / 2;
            thinkness = (end - i);
        }
        mat = createAirForHeight(height);
        name = "cell_" + to_string((int) ceil(height / meter));
        cellSolid = getCylinder(name, settings->geometrySettings->radius, thinkness);
        position = G4ThreeVector(0, 0, height + positionShift);
        cellLogic = new G4LogicalVolume(cellSolid, mat, name);
        cell = new G4PVPlacement(0, position, cellLogic, name, cloudLogic, false, indx);
        indx++;
    }

    return cloudLogic;
}

G4LogicalVolume *AragatsConstruction::CreateUniformCloud() {
    auto aragats_air_mean = createAirForHeight(settings->geometrySettings->height);
    double start = settings->aragatsSettings->low_boundary;
    double end = settings->aragatsSettings->high_boundary;
    auto cloudSolid = getCylinder("cloud", settings->geometrySettings->radius, end-start);

    auto cloudLogic = new G4LogicalVolume(cloudSolid, aragats_air_mean, "cloud");
    return cloudLogic;
}

G4LogicalVolume *AragatsConstruction::CreateCORSIKA() {
    double start = settings->aragatsSettings->low_boundary;
    double end = settings->aragatsSettings->high_boundary;
    double step = settings->aragatsSettings->pie_step;
    double positionShift = -1 * (start + end) / 2;
    double height;
    double thinkness, halfThinkness;
    G4Material *mat;
    G4Tubs *cellSolid;
    G4LogicalVolume *cellLogic;
    G4VPhysicalVolume *cell;
    G4ThreeVector position;
    string name;

    auto cloudSolid = getCylinder("cloud_pie", settings->geometrySettings->radius, end-start);
    auto cloudLogic = new G4LogicalVolume(cloudSolid, vacuum, "cloud_pie");

    int indx = 0;
    for (double i = start; i < end; i += step) {
        if (i + step <= end) {
            height = i + step / 2;
            thinkness = step;

        } else {
            height = i + (end - i) / 2;
            thinkness = (end - i);
        }
        mat = createAirForHeight(height);
        name = "cell_" + to_string((int) ceil(height / meter));
        cellSolid = getCylinder(name, settings->geometrySettings->radius, thinkness);
        position = G4ThreeVector(0, 0, height + positionShift);
        cellLogic = new G4LogicalVolume(cellSolid, mat, name);
        cell = new G4PVPlacement(0, position, cellLogic, name, cloudLogic, false, indx);
        indx++;
    }

    return cloudLogic;
}
