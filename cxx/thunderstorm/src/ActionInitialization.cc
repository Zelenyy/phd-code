#include <EventAction.hh>
#include <OneGenerationStackingAction.hh>
#include <Logger.hh>
#include <Dwyer2003StackingAction.hh>
#include "ActionInitialization.hh"
#include "GeneralParticleSource.hh"

using namespace std;

void ActionInitialization::Build() const {
//    GeneralParticleSource *generalParticleSource = new GeneralParticleSource();
//    SetUserAction(generalParticleSource);

    auto logger = Logger::instance();

    auto eventAction = new EventAction();
    SetUserAction(eventAction);

    if (settings->stacking == "one_generation"){
        SetUserAction(new OneGenerationStackingAction(settings));
        logger->print("Using stacking  action: one_generation");
    } else if (settings->stacking == "dwyer2003"){
        SetUserAction(new Dwyer2003StackingAction(settings));
        logger->print("Using stacking  action: dwyer2003");
    }


}
