#include "G4EmStandardPhysics.hh"
#include <G4EmStandardPhysics_option4.hh>
#include <G4EmStandardPhysics_option2.hh>
#include <G4EmStandardPhysics_option1.hh>
#include <G4EmStandardPhysics_option3.hh>
#include <G4EmLowEPPhysics.hh>
#include <G4DecayPhysics.hh>
#include <G4HadronPhysicsFTFP_BERT.hh>
#include <G4StoppingPhysics.hh>
#include <G4IonPhysics.hh>
#include <G4NeutronTrackingCut.hh>
#include "G4HadronElasticPhysics.hh"
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
    auto logger = Logger::instance();
    if (physics == "default" or physics == "standard"){
        RegisterPhysics(new G4EmStandardPhysics);
        logger->print("Using physics: standard");
    }
    else if (physics == "standard_opt_1"){
        RegisterPhysics(new G4EmStandardPhysics_option1);
        logger->print("Using physics: standard_opt_1");
    }
    else if (physics == "standard_opt_2"){
        RegisterPhysics(new G4EmStandardPhysics_option2);
        logger->print("Using physics: standard_opt_2");
    }
    else if (physics == "standard_opt_3"){
        RegisterPhysics(new G4EmStandardPhysics_option3);
        logger->print("Using physics: standard_opt_3");
    }
    else if (physics == "standard_opt_4"){
        RegisterPhysics(new G4EmStandardPhysics_option4);
        logger->print("Using physics: standard_opt_4");
    }
    else if (physics == "emlowepphysics") {
        RegisterPhysics(new G4EmLowEPPhysics);
        logger->print("Using physics: emlowepphysics");
    }
    else if (physics == "penelopa") {
        RegisterPhysics(new G4EmPenelopePhysics);
        logger->print("Using physics: penelopa");
    }
    else if (physics == "livermore"){
        RegisterPhysics(new G4EmLivermorePhysics);
        logger->print("Using physics: livermore");
    }
    else if (physics == "FTFP_BERT_opt4"){
        int ver = -1;
        defaultCutValue = 0.7*CLHEP::mm;
        // EM Physics
        RegisterPhysics( new G4EmStandardPhysics_option4(ver));

        // Synchroton Radiation & GN Physics
        RegisterPhysics( new G4EmExtraPhysics(ver) );

        // Decays
        RegisterPhysics( new G4DecayPhysics(ver) );

        // Hadron Elastic scattering
        RegisterPhysics( new G4HadronElasticPhysics(ver) );

        // Hadron Physics
        RegisterPhysics(  new G4HadronPhysicsFTFP_BERT(ver));

        // Stopping Physics
        RegisterPhysics( new G4StoppingPhysics(ver) );

        // Ion Physics
        RegisterPhysics( new G4IonPhysics(ver));

        // Neutron tracking cut
        RegisterPhysics( new G4NeutronTrackingCut(ver));
    }


}

