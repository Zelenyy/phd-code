//
// Created by zelenyy on 14.01.2020.
//

#ifndef PHD_CODE_DATA_HH
#define PHD_CODE_DATA_HH

#include <G4ThreeVector.hh>
#include "G4SystemOfUnits.hh"
#include "G4Track.hh"

using namespace std;

struct Numbers {
    int primary = 0;
    int gamma = 0;
    int electron = 0;
    int positron = 0;


    void clear() {
        primary = 0;
        electron = 0;
        positron = 0;
        gamma = 0;
    }

    void write(ofstream *fout) {
        *fout << primary << " " << gamma << " ";
        *fout << electron << " " << positron << " "<< endl;
    }
};

struct NamedNumbers{
    map<string, int> number;
    void clear(){
        for (auto it = number.begin(); it != number.end(); ++it){
            it->second = 0;
        }
    }

    void write_header(ofstream* fout){
        for (auto it : number){
            *fout<<it.first<<" ";
        }
        *fout<<endl;
    }

    void write(ofstream *fout){
        for (auto it : number){
            *fout<<it.second<<" ";
        }
        *fout<<endl;
    }
};

struct CylinderData {
    double energy;
    double theta;
    double radius;
    double z;

    void fillFromTrack(const G4Track *aTrack) {
        energy = aTrack->GetKineticEnergy() / MeV;
        const G4ThreeVector &momentumDir = aTrack->GetMomentumDirection();
        const G4ThreeVector &position = aTrack->GetPosition();
        theta = momentumDir.getTheta() / radian;
        radius = position.perp() / meter;
        z = position.getZ() / meter;
    }
};

struct CylinderIdData {
    int id;
    int parent_id;
    double energy;
    double theta;
    double radius;
    double z;

    void fillFromTrack(const G4Track *aTrack) {
        id = aTrack->GetTrackID();
        parent_id = aTrack->GetParentID();
        energy = aTrack->GetKineticEnergy() / MeV;
        const G4ThreeVector &momentumDir = aTrack->GetMomentumDirection();
        const G4ThreeVector &position = aTrack->GetPosition();
        theta = momentumDir.getTheta() / radian;
        radius = position.perp() / meter;
        z = position.getZ() / meter;
    }
};


struct TreeSocket{
    int id;
    int parent_id;
    int particle;
    double energy;
    double theta;
    double radius;
    double z;
    void fillFromStep(const G4Step *aStep){
        fillFromTrack(aStep->GetTrack());
    }

    void fillFromTrack(const G4Track *aTrack){
        id = aTrack->GetTrackID();
        parent_id = aTrack->GetParentID();
        particle = aTrack->GetDefinition()->GetPDGEncoding();
        energy = aTrack->GetKineticEnergy() / MeV;
        const G4ThreeVector &momentumDir = aTrack->GetMomentumDirection();
        const G4ThreeVector &position = aTrack->GetPosition();
        theta = momentumDir.getTheta() / radian;
        radius = position.perp() / meter;
        z = position.getZ() / meter;
    }
};
#endif //PHD_CODE_DATA_HH
