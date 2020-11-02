//
// Created by zelenyy on 09.02.2020.
//

#ifndef PHD_CODE_TRACKINGACTION_HH
#define PHD_CODE_TRACKINGACTION_HH


#include <G4UserTrackingAction.hh>
#include <SocketOutput.hh>
#include <DataFileManager.hh>
#include "DataThunderstorm.hh"
#include "thunderstorm.pb.h"
#include "Settings.hh"

class TrackingAction : public G4UserTrackingAction{
public:
    void PreUserTrackingAction(const G4Track *track) override;


    explicit TrackingAction(Settings* settings) : fSettings(settings){
        fTrackingSettings = settings->trackingSettings;
        if (fTrackingSettings->saveElectron || fTrackingSettings->saveGamma || fTrackingSettings->savePositron){
            data = new CylinderId;
            DataFileManager::instance()->registerDataContainer("tracking_post", data);
        }
    }

    void PostUserTrackingAction(const G4Track *track) override;

private:
    Settings* fSettings;
    TrackingSettings* fTrackingSettings;
    CylinderId *data;

    void PostGamma(const G4Track *track);
    void PostElectron(const G4Track *track);
    void PostPositron(const G4Track *track);
    void PostNeutron(const G4Track *track);

};


#endif //PHD_CODE_TRACKINGACTION_HH
