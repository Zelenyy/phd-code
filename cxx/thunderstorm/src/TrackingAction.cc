//
// Created by zelenyy on 09.02.2020.
//

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include <G4Positron.hh>
#include "TrackingAction.hh"
#include "G4AntiNeutron.hh"
#include "G4Neutron.hh"

void TrackingAction::PreUserTrackingAction(const G4Track *track) {
    G4UserTrackingAction::PreUserTrackingAction(track);

}

void TrackingAction::PostUserTrackingAction(const G4Track *track) {
    G4UserTrackingAction::PostUserTrackingAction(track);
    auto def = track->GetDefinition();
    if (def == G4Gamma::Definition()){
        return PostGamma(track);
    }
    if (def == G4Electron::Definition()){
        return PostElectron(track);
    }
    if (def == G4Positron::Definition()){
        return PostPositron(track);
    }
    if (def == G4Neutron::Definition() or def == G4AntiNeutron::Definition()){
        PostNeutron(track);
    }


}
void TrackingAction::PostNeutron(const G4Track *track) {
    if (fTrackingSettings->saveNeutron){
        data->addTrack(track);
    }
}

void TrackingAction::PostGamma(const G4Track *track) {
    if (fTrackingSettings->saveGamma){
        data->addTrack(track);
    }
}

void TrackingAction::PostElectron(const G4Track *track) {
    if (fTrackingSettings->saveElectron){
        data->addTrack(track);
    }
}

void TrackingAction::PostPositron(const G4Track *track) {
    if (fTrackingSettings->savePositron){
        data->addTrack(track);
    }
}
