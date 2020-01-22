//
// Created by mihail on 24.05.17.
//

#include "GeneralParticleSource.hh"
#include "G4GeneralParticleSource.hh"

using namespace CLHEP;

GeneralParticleSource::GeneralParticleSource() {
    particleSource = new G4GeneralParticleSource();
}

GeneralParticleSource::~GeneralParticleSource() {
    delete particleSource;
}

void GeneralParticleSource::GeneratePrimaries(G4Event *event) {
    particleSource->GeneratePrimaryVertex(event);
}
