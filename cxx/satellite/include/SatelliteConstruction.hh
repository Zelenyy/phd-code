//
// Created by zelenyy on 15.05.2021.
//

#ifndef PHD_CODE_SATELLITECONSTRUCTION_HH
#define PHD_CODE_SATELLITECONSTRUCTION_HH

#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include "Settings.hh"
#include "DataSatellite.hh"
#include "G4SDManager.hh"

class WithWrapperScoredDetector : public G4VSensitiveDetector {
public:

    WithWrapperScoredDetector(Settings* settings) : settings(settings), G4VSensitiveDetector("wrapper_deposit"){
        deposit.resize(number);
        wrapper_deposit.resize(number);
        DataSatellite::instance()->wrapperRun = new satellite::Run();
        wrapperRun = DataSatellite::instance()->wrapperRun;
    }

    void Initialize(G4HCofThisEvent *event) override {
//        G4VSensitiveDetector::Initialize(event);
        for (int i = 0; i< number; ++i) {
            deposit[i] = 0.0;
            wrapper_deposit[i] = 0.0;
        }
    }

    void EndOfEvent(G4HCofThisEvent *event) override {
//        G4VSensitiveDetector::EndOfEvent(event);
        if (settings->scoredDetectorMode == ScoredDetectorMode::single){
            auto run = DataSatellite::instance()->run;
            auto event = run->add_event();
            auto wrapper_event = wrapperRun->add_event();
            for (int i = 0; i< number; ++i) {
                event->add_deposit(deposit[i]);
                wrapper_event->add_deposit(wrapper_deposit[i]);
            }
        }
    }

protected:
    Settings *settings;
    int number = 20;
    vector<double> deposit;
    vector<double> wrapper_deposit;
    satellite::Run* wrapperRun;
    G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) override {
        auto volume = aStep->GetTrack()->GetVolume();
        auto mother_volume = aStep->GetTrack()->GetTouchable()->GetVolume(1);
//        cout<<mother_volume->GetName()<< " " << mother_volume->GetCopyNo()<<endl;
        int id = mother_volume->GetCopyNo();
        if (volume->GetName() == "cell") {
            deposit[id] += aStep->GetTotalEnergyDeposit() / MeV;
        }
        if (volume->GetName() == "cell_wrapper"){
            wrapper_deposit[id] += aStep->GetTotalEnergyDeposit() / MeV;
        }
        return 0;
    }

};


class SatelliteConstruction: public G4VUserDetectorConstruction{
public:
    SatelliteConstruction(Settings *settings) : settings(settings) {
        InitializeMaterials();
        logger = Logger::instance();
        geometrySettings = settings->geometrySettings;
        logger->print("Create satellite construction class");
    };

    G4VPhysicalVolume *Construct() override {
        auto world = constuctWorld();
        auto logicWorld = world->GetLogicalVolume();



        auto cell_case_solid = getCylinder("cell_case", radius, cell_case_length);
        auto cell_solid = getCylinder("cell", radius, cell_length);
        auto cell_wrapper_solid = getCylinder("cell_wrapper",radius, cell_wrapper_length);

        auto cell_case_logic = new G4LogicalVolume(cell_case_solid, vacuum, "cell_case");

        cell_logic = new G4LogicalVolume(cell_solid, cell, "cell");
        cell_wrapper_logic = new G4LogicalVolume(cell_wrapper_solid, cell_wrapper, "cell_wrapper");
        auto cell_phys = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, -cell_wrapper_length/2), cell_logic, "cell", cell_case_logic, false, 0);
        auto cell_wrapper_phys = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, cell_length/2), cell_wrapper_logic, "cell_wrapper", cell_case_logic, false, 0);

        for (int i = 0; i<number; ++i){
            auto cell_case_phys = new G4PVPlacement(0, G4ThreeVector(0.0, 0.0, -i*cell_case_length - cell_case_length/2), cell_case_logic, "cell_case", logicWorld, true, i);
        }


        return world;
    }

    void ConstructSDandField() override {
        G4VUserDetectorConstruction::ConstructSDandField();
        auto det = new WithWrapperScoredDetector(settings);
        G4SDManager::GetSDMpointer()->AddNewDetector(det);
        cell_wrapper_logic->SetSensitiveDetector(det);
        cell_logic->SetSensitiveDetector(det);
    }
protected:
    Settings *settings;
    GeometrySettings* geometrySettings;
    Logger *logger;


    G4Material *vacuum;
    G4Material *cell;
    G4Material *cell_wrapper;
    G4double world_radius = 10 * cm;
    G4double world_sizeZ = 40 * cm;
    G4bool checkOverlaps = true;

    double cell_length = 4*mm;
    double cell_wrapper_length = 2*175*micrometer;
    double cell_case_length = cell_length + cell_wrapper_length;
    double radius = 30*mm;
    int number = 20;

    G4LogicalVolume *cell_logic;
    G4LogicalVolume *cell_wrapper_logic;

    void InitializeMaterials() {
        auto nist = G4NistManager::Instance();
        vacuum = nist->FindOrBuildMaterial("G4_Galactic");
        cell = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
        cell_wrapper = nist->FindOrBuildMaterial("G4_POLYETHYLENE");

        double tyvek_density = 0.3314*(gram/cm3);
        cell_wrapper = new G4Material("TYVEK", tyvek_density, cell_wrapper);

    };

    G4Tubs * getCylinder(const string &name, double radius, double heigth) {
        return new G4Tubs(G4String(name), 0, radius, 0.5 * heigth, 0, twopi);
    }

    G4VPhysicalVolume * constuctWorld() {



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

};

#endif //PHD_CODE_SATELLITECONSTRUCTION_HH



