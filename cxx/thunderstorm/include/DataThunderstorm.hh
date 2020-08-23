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

class CylinderId : public ProtoWrapper<thunderstorm::CylinderIdList>{
public:
    void initializeEvent(int eventID) override {
        ProtoWrapper::initializeEvent(eventID);
        protoList->set_eventid(eventID);
    }

    void addTrack(const G4Track *track) override {
        auto data = protoList->add_cylinderid();
        data->set_id(track->GetTrackID());
        data->set_parent_id( track->GetParentID());
        data->set_particle( track->GetDefinition()->GetPDGEncoding());
        data->set_energy(track->GetKineticEnergy() / MeV);
        const G4ThreeVector &momentumDir = track->GetMomentumDirection();
        const G4ThreeVector &position = track->GetPosition();
        data->set_theta(momentumDir.getTheta() / radian);
        data->set_radius(position.perp() / meter);
        data->set_z(position.getZ() / meter);
        data->set_time(track->GetGlobalTime() / ns);
        ProtoWrapper::addTrack(track);
    }
};

class ParticleDetectorList : public  ProtoWrapper<thunderstorm::ParticleDetectorList>{
public:
    void initializeEvent(int eventID) override {
        ProtoWrapper::initializeEvent(eventID);
        protoList->set_eventid(eventID);
    }

    void addTrack(const G4Track *track) override {
        auto data = protoList->add_data();
        data->set_particle( track->GetDefinition()->GetPDGEncoding());
        data->set_energy(track->GetKineticEnergy() / MeV);
        const G4ThreeVector &momentumDir = track->GetMomentumDirection();
        const G4ThreeVector &position = track->GetPosition();
        data->set_theta(momentumDir.getTheta() / radian);
        data->set_radius(position.perp() / meter);
        data->set_time(track->GetGlobalTime() / ns);
        ProtoWrapper::addTrack(track);
    }

};

#endif //PHD_CODE_DATATHUNDERSTORM_HH
