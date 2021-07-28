//
// Created by zelenyy on 26.02.2020.
//

#ifndef PHD_CODE_RUNACTION_HH
#define PHD_CODE_RUNACTION_HH


#include <G4UserRunAction.hh>
#include "Settings.hh"
#include <SocketOutput.hh>
class RunAction: public G4UserRunAction {
public:

    explicit RunAction(Settings* settings, SocketOutput *socket): socket(socket){
        fSettings = settings;
    }

    void EndOfRunAction(const G4Run *aRun) override;

    void BeginOfRunAction(const G4Run *aRun) override;

    ~RunAction() override{
        if (fSettings->outputMode == OutputMode::socket_client){
            socket->closeSocket();
        }

    }
private:
    Settings * fSettings;
    SocketOutput *socket;
};


#endif //PHD_CODE_RUNACTION_HH
