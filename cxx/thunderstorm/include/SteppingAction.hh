//
// Created by zelenyy on 10.02.2020.
//

#ifndef PHD_CODE_STEPPINGACTION_HH
#define PHD_CODE_STEPPINGACTION_HH


#include <G4UserSteppingAction.hh>
#include "G4Step.hh"
#include "Settings.hh"

class SteppingAction : public G4UserSteppingAction {
public:
    explicit SteppingAction(Settings* settings) : fSettings(settings){};
    void UserSteppingAction(const G4Step *step) override;
private:
    Settings* fSettings;
};

class CriticalEnergySteppingAction : public G4UserSteppingAction {
public:
    explicit CriticalEnergySteppingAction(Settings* settings) : fSettings(settings){};
    void UserSteppingAction(const G4Step *step) override {
        {
            int id = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
            if (id != eventId){
                deltaEnergy = 0.0;
                eventId = id;
            }
        }
        if (step->GetTrack()->GetKineticEnergy() < fSettings->minimal_energy){
            step->GetTrack()->SetTrackStatus(fStopAndKill);
        }
        if (isFirst){
            isFirst = false;
            initEnergy = step->GetTrack()->GetKineticEnergy();
            initEnergy -= step->GetDeltaEnergy();
        }
        cout<<step->GetTrack()->GetKineticEnergy()<<'\t'
            <<step->GetTotalEnergyDeposit()<<'\t'
            <<step->GetDeltaEnergy()<<endl;
        deltaEnergy += abs(step->GetDeltaEnergy());
        if (step->GetTrack()->GetKineticEnergy() > 3*initEnergy){
            step->GetTrack()->SetTrackStatus(fStopAndKill);
        } else {
            if (deltaEnergy > 10*initEnergy){
                step->GetTrack()->SetTrackStatus(fStopAndKill);
            }
        }

    }

private:
    Settings* fSettings;
    int eventId = -1;
    bool isFirst = true;
    double initEnergy;
    double deltaEnergy = 0.0;
};


#endif //PHD_CODE_STEPPINGACTION_HH
