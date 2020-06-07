//
// Created by zelenyy on 22.11.2019.
//

#ifndef GEANT4_DFCLIENT_G4DFCLIENT_HH
#define GEANT4_DFCLIENT_G4DFCLIENT_HH

#include <string>
#include "DetectorConstruction.hh"
#include "G4VUserPhysicsList.hh"
#include "G4VUserActionInitialization.hh"
#define SO_REUSEPORT 15
class G4DFClientMessenger;
class G4RunManager;

using namespace std;


class G4DFClient {


private:
    G4DFClient();
    G4DFClient(G4DFClient const&);
    G4DFClient& operator=(G4DFClient const&);
public:
    DetectorConstruction* massWorld;
    G4RunManager *runManager;
    static G4DFClient* instance(){
        static G4DFClient client;
        return &client;
    }

    void setup(G4VUserPhysicsList * physList, G4VUserActionInitialization * actions = nullptr);
    void initialize();
    int read(istream& input);

    void startSocketServer();


    void stop(){
        delete runManager;
    }
    string getProject(){
        return project;
    }

    void setProject(string project){
        this->project = project;
    }

    string getGDML(){
        return gdml;
    }

    void setGDML(string gdml){
        this->gdml = gdml;
    }

    void setSeed(long seed){
        this->seed = seed;
    }

    void setPort(int port){
        this->port = port;
    }

private:
    int argc;
    char **argv;
    string project;
    string gdml;
    long seed = -1;
    int port = 8777;
    int new_socket{};
    int server_fd{};
    G4DFClientMessenger* messenger;
    int countRead = 1;

    string macros = "";

//    void parse_input(int argc_, char **argv_);
// Configurable parameter
public:
    int lengthOfCommand = 1000;
};


#endif //GEANT4_DFCLIENT_G4DFCLIENT_HH
