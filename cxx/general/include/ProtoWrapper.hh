//
// Created by zelenyy on 22.08.2020.
//

#ifndef PHD_CODE_PROTOWRAPPER_HH
#define PHD_CODE_PROTOWRAPPER_HH


#include "iostream"
#include "G4Track.hh"

class DataContainer{
public:
    virtual void initializeRun(std::ostream* out) = 0;
    virtual void initializeEvent(int eventID) = 0;
    virtual void addTrack(const G4Track* track) = 0;
    virtual void finishEvent() = 0;
    virtual void finishRun() = 0;
};

template <class T>
class ProtoWrapper : public DataContainer{
protected:
    T* protoList = nullptr;
    std::ostream* out;
    int count = 0;
    void write(){
        if (protoList != nullptr){
            long size = protoList->ByteSizeLong();
            if (size != 0){
                out->write(reinterpret_cast<char*>(&size), sizeof size);
                protoList->SerializeToOstream(out);
                protoList->Clear();
                count = 0;
            }

        }
    }

public:

    explicit ProtoWrapper()= default;

    void initializeRun(std::ostream* out) override{
        this->out = out;
        if (protoList != nullptr){
            delete protoList;
        }
        protoList = new T;
    }

    void initializeEvent(int eventID) override{}

    void addTrack(const G4Track* track) override{
        count++;
        if (count > 100000){
            write();
        }
    };

    void finishEvent() override{
        write();
    }

    void finishRun() override{
        write();
    }
};

#endif //PHD_CODE_PROTOWRAPPER_HH
