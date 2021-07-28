#include <Logger.hh>
#include <RunAction.hh>
#include <DataSatellite.hh>
#include <SocketOutput.hh>
#include "ActionInitialization.hh"
#include "GeneralParticleSource.hh"

using namespace std;

void ActionInitialization::Build() const {
    auto logger = Logger::instance();
    DataSatellite::instance()->initialize(settings);
    SocketOutput *socket = nullptr;
    if (settings->outputMode == OutputMode::socket_client){
        socket = new SocketOutput("deposit", settings->port);
    }
    SetUserAction(new RunAction(settings, socket));
}
