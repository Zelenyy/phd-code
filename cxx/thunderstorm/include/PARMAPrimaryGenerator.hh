//
// Created by zelenyy on 09.07.2020.
//

#ifndef PHD_CODE_PARMAPRIMARYGENERATOR_HH
#define PHD_CODE_PARMAPRIMARYGENERATOR_HH

#include "Parma.hh"
#include "G4ParticleGun.hh"
#include "G4MuonMinus.hh"
#include "G4MuonPlus.hh"

class PARMAPrimaryGenerator : public G4VUserPrimaryGeneratorAction{
private:
    PARMAGenerator* generator;
    G4ParticleGun* gun;
    Settings* settings;
public:
    PARMAPrimaryGenerator(Settings* settings) : settings(settings) {
        generator = new PARMAGenerator();
        gun = new G4ParticleGun();
        gun->SetParticleDefinition(G4MuonMinus::Definition());
    }

    void GeneratePrimaries(G4Event *anEvent) override {
        auto parma_event = generator->generate();
        gun->SetParticleMomentumDirection(parma_event.direction);
        auto shift = settings->parmaSettings->position;
        gun->SetParticlePosition(shift + parma_event.position);
        gun->SetParticleEnergy(parma_event.Energy);
        gun->SetParticleDefinition(settings->parmaSettings->particle->get());
        gun->GeneratePrimaryVertex(anEvent);
    }

};

#endif //PHD_CODE_PARMAPRIMARYGENERATOR_HH
