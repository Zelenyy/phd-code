#include <EventAction.hh>
#include <Logger.hh>
#include <Dwyer2003StackingAction.hh>
#include <StackingAction.hh>
#include <ParticleCylinderStacking.hh>
#include <TrackingAction.hh>
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
    StackingAction *stackingAction;
    if (stackingSettings->type == StackingType::simple){
        stackingAction = new StackingAction(settings);
        SetUserAction(stackingAction);
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
    SteppingAction* steppingAction;
//    if (steppingSettings->type == SteppingType::simple) {
//        steppingAction = new SteppingAction(settings);
//        logger->print("Using stepping  action: default");
//    } else if (steppingSettings->type == SteppingType::critical_energy){
//        steppingAction =new CriticalEnergySteppingAction(settings);
//        logger->print("Using stepping  action: critical_energy");
//    }
    steppingAction = new SteppingAction(settings);
    SetUserAction(steppingAction);


    TrackingAction *trackingAction;
    trackingAction = new TrackingAction(settings);
    SetUserAction(trackingAction);


    if (settings->superviseTree){
        auto electronCounter = new ElectronsCounter(settings, "electron");
        eventAction->electronCounter = electronCounter;
        trackingAction->electronCounter = electronCounter;
        steppingAction->electronsCounter = electronCounter;

    }

}
