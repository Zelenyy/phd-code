//
// Created by mihail on 24.05.17.
//

#ifndef GEANT4_GENERALPARTICLESOURCE_HH
#define GEANT4_GENERALPARTICLESOURCE_HH

#include "G4VUserPrimaryGeneratorAction.hh"

class G4Event;

class G4GeneralParticleSource;

class GeneralParticleSource : public G4VUserPrimaryGeneratorAction {
public:
    GeneralParticleSource();

    ~GeneralParticleSource() override;

    void GeneratePrimaries(G4Event *event) override;

private:
//    G4ParticleGun* particleGun;
    G4GeneralParticleSource *particleSource;
};


#endif //GEANT4_GENERALPARTICLESOURCE_HH
