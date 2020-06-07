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
    const double high_boundary = 4250*meter;
    const double low_boundary = 3250*meter;
    const double observed_level = 3200*meter;
    G4LogicalVolume *detectorLogic = nullptr;
    G4LogicalVolume *cloudLogic = nullptr;
};

#endif //PHD_CODE_THUNDERSTORMCONSTRUCTION_HH
