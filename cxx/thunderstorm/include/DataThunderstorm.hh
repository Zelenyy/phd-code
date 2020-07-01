//
// Created by zelenyy on 22.02.2020.
//

#ifndef PHD_CODE_DATATHUNDERSTORM_HH
#define PHD_CODE_DATATHUNDERSTORM_HH

#include "G4Track.hh"
#include "histogram.pb.h"
#include "thunderstorm.pb.h"
#include "G4SystemOfUnits.hh"
#include "DataFileManager.hh"
using namespace CLHEP;

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
    void saveDwyer2003StackingAction(){
        if (gammaData != nullptr){
            int size = gammaData->ByteSize();
            auto fout = DataFileManager::instance()->getBinaryFile("gammaSeed");
            fout->write(reinterpret_cast<char*>(&size), sizeof size);
            gammaData->SerializeToOstream(fout);
            gammaData->Clear();
        }
        if (positronData != nullptr){
            int size =positronData->ByteSize();
            auto fout = DataFileManager::instance()->getBinaryFile("positronSeed");
            fout->write(reinterpret_cast<char*>(&size), sizeof size);
            positronData->SerializeToOstream(fout);
            positronData->Clear();
        }
    }
    // TreeTracking
    thunderstorm::CylinderIdList* treeTrackingData = nullptr;
    void initTreeTracking(){
        treeTrackingData = new thunderstorm::CylinderIdList;
    }
    void saveTreeTracking(){
//        if (treeTrackingData != nullptr){
//            int size = treeTrackingData->ByteSize();
//            auto fout = DataFileManager::instance()->getBinaryFile();
//            fout->write(reinterpret_cast<char*>(&size), sizeof size);
//            treeTrackingData->SerializeToOstream(fout);
//            treeTrackingData->Clear();
//        }
        saveProtoChunk("treeTracking", treeTrackingData);
    }

    static void saveProtoChunk(const std::string& filename, ::google::protobuf::Message* chunk){
        if (chunk != nullptr){
            int size = chunk->ByteSize();
            auto fout = DataFileManager::instance()->getBinaryFile(filename);
            fout->write(reinterpret_cast<char*>(&size), sizeof size);
            chunk->SerializeToOstream(fout);
            chunk->Clear();
        }
    }

    // ParticleDetector
    thunderstorm::ParticleDetectorList* particleDetectorList;
    void initParticleDetector(){
        particleDetectorList = new thunderstorm::ParticleDetectorList();
    }

    void fillParticleDetector(thunderstorm::ParticleDetectorData* data, const G4Track *aTrack){
        data->set_particle( aTrack->GetDefinition()->GetPDGEncoding());
        data->set_energy(aTrack->GetKineticEnergy() / MeV);
        const G4ThreeVector &momentumDir = aTrack->GetMomentumDirection();
        const G4ThreeVector &position = aTrack->GetPosition();
        data->set_theta(momentumDir.getTheta() / radian);
        data->set_radius(position.perp() / meter);
        data->set_time(aTrack->GetGlobalTime() / ns);
//        data.set
    }

    void saveParticleDetector(){
        saveProtoChunk("particle_detector", particleDetectorList);
    }


    // Concentration
    histogram::Histogram4D* histogram4D;
    void initConcentration(){
        histogram4D = new histogram::Histogram4D();
    }

    void saveConcentration(){
        saveProtoChunk("concentration", histogram4D);
    }

    void EndEvent(){
        saveDwyer2003StackingAction();
        saveTreeTracking();
        saveParticleDetector();
        saveConcentration();
    }

private:
    DataThunderstorm(){
        predictorHist = new histogram::Histogram2DList();
    };

    DataThunderstorm(DataThunderstorm const &) = delete;

    DataThunderstorm &operator=(DataThunderstorm const &) = delete;
};


#endif //PHD_CODE_DATATHUNDERSTORM_HH
