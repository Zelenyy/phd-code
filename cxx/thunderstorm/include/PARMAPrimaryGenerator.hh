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
    parma::Generator* generator;
    G4ParticleGun* gun;
    Settings* settings;
    double theta;
public:
    PARMAPrimaryGenerator(Settings* settings) : settings(settings) {
        parma::Coord coord = {40.0, 44.0, settings->aragatsSettings->high_boundary / kilometer};
        parma::Date date = {2019, 6, 15};

        parma::Particle particle;
        G4String name = settings->parmaSettings->particle->getBackField();
        if (name == "mu-"){
            particle = parma::Particle::mu_minus;
        } else if (name == "mu+"){
            particle = parma::Particle::mu_plus;
        }
        double height = settings->aragatsSettings->high_boundary - settings->aragatsSettings->low_boundary;
        theta = atan(height/4.5*km);
        auto model = new parma::Model(".");
        generator = new parma::Generator(particle, date, coord, model);
        gun = new G4ParticleGun();
        gun->SetParticleDefinition(G4MuonMinus::Definition());
    }

    void GeneratePrimaries(G4Event *anEvent) override {

        parma::Event parma_event;
        do{
            parma_event = generator->generate();
        } while (parma_event.direction.theta() < pi - theta);

        gun->SetParticleMomentumDirection(parma_event.direction);
        auto shift = settings->parmaSettings->position;
        gun->SetParticlePosition(shift + parma_event.position);
        gun->SetParticleEnergy(parma_event.Energy);
        gun->SetParticleDefinition(settings->parmaSettings->particle->get());
        gun->GeneratePrimaryVertex(anEvent);
    }

};

#endif //PHD_CODE_PARMAPRIMARYGENERATOR_HH
