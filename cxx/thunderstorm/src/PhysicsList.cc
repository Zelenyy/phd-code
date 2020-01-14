#include "G4EmStandardPhysics.hh"
#include <G4EmStandardPhysics_option4.hh>
#include "PhysicsList.hh"
#include "string"
//Physics process classes
#include "G4EmExtraPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "Logger.hh"
using namespace CLHEP;
using namespace std;
PhysicsList::PhysicsList(string physics) {
    if (physics == "default" or physics == "standard"){
        RegisterPhysics(new G4EmStandardPhysics);
    }
    else if (physics == "standard_opt_4"){
        RegisterPhysics(new G4EmStandardPhysics_option4);
    }
    else if (physics == "penelopa") {
        RegisterPhysics(new G4EmPenelopePhysics);
    }
    else if (physics == "livermore"){
        RegisterPhysics(new G4EmLivermorePhysics);
    }
    Logger::instance()->print("Using physics: "  + physics);

}

