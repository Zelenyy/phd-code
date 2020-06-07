//
// Created by zelenyy on 05.06.2020.
//

#ifndef PHD_CODE_THUNDERSTORMCONSTRUCTION_HH
#define PHD_CODE_THUNDERSTORMCONSTRUCTION_HH


#include <G4VUserDetectorConstruction.hh>
#include <G4PVPlacement.hh>
#include <G4NistManager.hh>
#include "Settings.hh"

namespace ISA {

    const G4double t0 = 288.15 * kelvin;
    const  G4double temperatureGrad = -0.0065 * kelvin / m; //температурный градиент
    const  G4double p0 = 101325 * pascal;
    const  G4double g = 9.80665 * m / (s * s);
    const  G4double M = 0.0289644 * kg / mole;
    const  G4double R = 8.31447 * joule / (kelvin * mole);
};

class ThunderstormConstruction : public G4VUserDetectorConstruction {
public:
    explicit ThunderstormConstruction(Settings *settings) : settings(settings) {
        InitializeMaterials();
        logger = Logger::instance();
    };

protected:
    Settings *settings;
    Logger* logger;
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

class AragatsConstruction  : public ThunderstormConstruction{

public:

    explicit AragatsConstruction(Settings *settings) : ThunderstormConstruction(settings){};

    G4VPhysicalVolume *Construct() override;

    void ConstructSDandField() override;

private:

    G4LogicalVolume *detectorLogic = nullptr;
    G4LogicalVolume *cloudLogic = nullptr;

    G4LogicalVolume * CreateUniformCloud(){
        auto aragats_air_mean = createAirForHeight(settings->geometrySettings->height);
        double start = settings->aragatsSettings->low_boundary;
        double end = settings->aragatsSettings->high_boundary;
        auto cloudSolid = getCylinder("cloud", settings->geometrySettings->radius, end-start);

        auto cloudLogic = new G4LogicalVolume(cloudSolid, aragats_air_mean, "cloud");
        return cloudLogic;
    }

    G4LogicalVolume * CreateAirPie(){
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

};

#endif //PHD_CODE_THUNDERSTORMCONSTRUCTION_HH
