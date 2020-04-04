//
// Created by zelenyy on 09.02.2020.
//

#include "TreeTrackingAction.hh"

void TreeTrackingAction::PreUserTrackingAction(const G4Track *track) {
    G4UserTrackingAction::PreUserTrackingAction(track);
    auto cylinderId = data->add_cylinderid();
    DataThunderstorm::fillCylinderId(cylinderId, track);
    if (data->cylinderid_size() > 100000){
        dataThunderstorm->saveTreeTracking();
    }
}
