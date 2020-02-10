//
// Created by zelenyy on 09.02.2020.
//

#include "TreeSocketTrackingAction.hh"

void TreeSocketTrackingAction::PreUserTrackingAction(const G4Track *track) {
    G4UserTrackingAction::PreUserTrackingAction(track);
    data.fillFromTrack(track);
    socketOutput->addData(data);
}
