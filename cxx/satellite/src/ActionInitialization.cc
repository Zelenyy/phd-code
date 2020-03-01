#include <Logger.hh>
#include <RunAction.hh>
#include "ActionInitialization.hh"
#include "GeneralParticleSource.hh"

using namespace std;

void ActionInitialization::Build() const {
    auto logger = Logger::instance();
    SetUserAction(new RunAction(settings));
}
