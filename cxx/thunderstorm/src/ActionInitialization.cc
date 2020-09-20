#include <EventAction.hh>
#include <Logger.hh>
#include <Dwyer2003StackingAction.hh>
#include <StackingAction.hh>
#include <ParticleCylinderStacking.hh>
#include <TreeTrackingAction.hh>
#include <SteppingAction.hh>
#include "ActionInitialization.hh"
#include "GeneralParticleSource.hh"
#include "ServerSettings.hh"
#include "RunAction.hh"

using namespace std;

void ActionInitialization::Build() const {
    if (settings->generator == PrimaryGeneratorType::gps){
        GeneralParticleSource *generalParticleSource = new GeneralParticleSource();
        SetUserAction(generalParticleSource);
    } else if (settings->pgSubType == ThunderstomPGSubType::parma){
        auto generator = new PARMAPrimaryGenerator(settings);
        SetUserAction(generator);
    }

    auto logger = Logger::instance();
    auto runAction = new RunAction(settings);
    SetUserAction(runAction);
    auto eventAction = new EventAction(settings);
    SetUserAction(eventAction);


    auto stackingSettings = settings->stackingSettings;
    if (stackingSettings->type == StackingType::simple){
        SetUserAction(new StackingAction(settings));
        logger->print("Using stacking  action: default simple");
    }
//     if (settings->stacking == "dwyer2003") {
//        settings->particlePredictor = new ParticlePredictor;
//        SetUserAction(new Dwyer2003StackingAction(settings));
//        logger->print("Using stacking  action: dwyer2003");
//    } else if (settings->stacking == "particle_cylinder") {
//        SetUserAction(new ParticleCylinderStacking(settings));
//        logger->print("Using stacking  action: particle_cylinder");
//    }
    auto steppingSettings = settings->steppingSettings;
    if (steppingSettings->type == SteppingType::simple) {
        SetUserAction(new SteppingAction(settings));
        logger->print("Using stepping  action: default");
    } else if (steppingSettings->type == SteppingType::critical_energy){
        SetUserAction(new CriticalEnergySteppingAction(settings));
        logger->print("Using stepping  action: critical_energy");
    }
//    else if (settings->stepping == "tree_socket") {
//        SetUserAction(new TreeSocketSteppingAction());
//        logger->print("Using stepping  action: tree_socket");
//    }

    if (settings->tracking == "tree") {
        SetUserAction(new TreeTrackingAction());
        logger->print("Using tracking  action: tree_socket");
    }

}
