//
// Created by zelenyy on 09.02.2020.
//

#ifndef PHD_CODE_TREESOCKETTRACKINGACTION_HH
#define PHD_CODE_TREESOCKETTRACKINGACTION_HH


#include <G4UserTrackingAction.hh>
#include <SocketOutput.hh>
#include <DataFileManager.hh>
#include "Data.hh"

class TreeSocketTrackingAction : public G4UserTrackingAction{
public:
    void PreUserTrackingAction(const G4Track *track) override;

    TreeSocketTrackingAction(){
        socketOutput = DataFileManager::instance()->GetSocketOutput<TreeSocket>("TreeTracking");
    }

private:
    TreeSocket data;
    SocketOutput<TreeSocket> * socketOutput;
};


#endif //PHD_CODE_TREESOCKETTRACKINGACTION_HH
