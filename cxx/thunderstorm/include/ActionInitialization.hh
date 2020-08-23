//
// Created by mihail on 24.05.17.
//

#ifndef GEANT4_ACTIONINITIALIZATION_HH
#define GEANT4_ACTIONINITIALIZATION_HH

#include "G4VUserActionInitialization.hh"
#include "Settings.hh"
#include "PARMAPrimaryGenerator.hh"

class ActionInitialization : public G4VUserActionInitialization {
public:
    explicit ActionInitialization(Settings* pSettings) : G4VUserActionInitialization(), settings(pSettings) {};

    ~ActionInitialization() override = default;

    void Build() const override;

private:
    Settings* settings;

};


#endif //GEANT4_ACTIONINITIALIZATION_HH
