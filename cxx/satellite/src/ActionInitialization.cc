#include <Logger.hh>
#include <RunAction.hh>
#include <DataSatellite.hh>
#include "ActionInitialization.hh"
#include "GeneralParticleSource.hh"

using namespace std;

void ActionInitialization::Build() const {
    auto logger = Logger::instance();
    DataSatellite::instance()->initialize(settings);
    SetUserAction(new RunAction(settings));
}
