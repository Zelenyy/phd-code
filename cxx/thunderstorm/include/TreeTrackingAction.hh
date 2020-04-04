//
// Created by zelenyy on 09.02.2020.
//

#ifndef PHD_CODE_TREETRACKINGACTION_HH
#define PHD_CODE_TREETRACKINGACTION_HH


#include <G4UserTrackingAction.hh>
#include <SocketOutput.hh>
#include <DataFileManager.hh>
#include "DataThunderstorm.hh"
#include "thunderstorm.pb.h"

class TreeTrackingAction : public G4UserTrackingAction{
public:
    void PreUserTrackingAction(const G4Track *track) override;

    TreeTrackingAction(){
        dataThunderstorm = DataThunderstorm::instance();
        dataThunderstorm->initTreeTracking();
        data = dataThunderstorm->treeTrackingData;
    }

private:
    DataThunderstorm *dataThunderstorm;
    thunderstorm::CylinderIdList* data;
};


#endif //PHD_CODE_TREETRACKINGACTION_HH
