//
// Created by zelenyy on 09.02.2020.
//

#include "TreeTrackingAction.hh"

void TreeTrackingAction::PreUserTrackingAction(const G4Track *track) {
    G4UserTrackingAction::PreUserTrackingAction(track);
    data->addTrack(track);
}
