//
// Created by zelenyy on 22.02.2020.
//

#ifndef PHD_CODE_DATATHUNDERSTORM_HH
#define PHD_CODE_DATATHUNDERSTORM_HH

#include "histogram.pb.h"
#include "thunderstorm.pb.h"

class DataThunderstorm {
public:
    histogram::Histogram2DList* predictorHist;
    static DataThunderstorm *instance() {
        static DataThunderstorm dataThunderstorm;

        return &dataThunderstorm;
    }

    static void fillCylinderId(thunderstorm::CylinderId* data, const G4Track *aTrack ){
        data->set_id(aTrack->GetTrackID());
        data->set_parent_id( aTrack->GetParentID());
        data->set_particle( aTrack->GetDefinition()->GetPDGEncoding());
        data->set_energy(aTrack->GetKineticEnergy() / MeV);
        const G4ThreeVector &momentumDir = aTrack->GetMomentumDirection();
        const G4ThreeVector &position = aTrack->GetPosition();
        data->set_theta(momentumDir.getTheta() / radian);
        data->set_radius(position.perp() / meter);
        data->set_z(position.getZ() / meter);
    }

public:
    //For dwyer2003Stacking
    thunderstorm::CylinderIdList* gammaData = nullptr;
    thunderstorm::CylinderIdList* positronData = nullptr;
    void initDwyer2003StackingActionData(){
        gammaData = new thunderstorm::CylinderIdList;
        positronData = new thunderstorm::CylinderIdList;
    }
    void endEventDwyer2003StackingAction(){
        if (gammaData != nullptr){
            int size = gammaData->ByteSize();
            auto fout = DataFileManager::instance()->getBinaryFile("gammaSeed");
            fout->write(reinterpret_cast<char*>(&size), sizeof size);
            gammaData->SerializeToOstream(fout);
        }
        if (positronData != nullptr){
            int size =positronData->ByteSize();
            auto fout = DataFileManager::instance()->getBinaryFile("positronSeed");
            fout->write(reinterpret_cast<char*>(&size), sizeof size);
            positronData->SerializeToOstream(fout);
        }
    }

    void EndEvent(){
        endEventDwyer2003StackingAction();
    }

private:
    DataThunderstorm(){
        predictorHist = new histogram::Histogram2DList();
    };

    DataThunderstorm(DataThunderstorm const &) = delete;

    DataThunderstorm &operator=(DataThunderstorm const &) = delete;
};


#endif //PHD_CODE_DATATHUNDERSTORM_HH
