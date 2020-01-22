#ifndef GAMMAPHYSICSLIST_HH
#define GAMMAPHYSICSLIST_HH

#include "G4VModularPhysicsList.hh"
using namespace std;

class PhysicsList: public G4VModularPhysicsList
{
public:
    PhysicsList(string physics = "default");
    ~PhysicsList(){};
};

#endif /* GAMMAPHYSICSLIST_HH */

