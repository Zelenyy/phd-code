//
// Created by zelenyy on 24.02.2020.
//

#ifndef PHD_CODE_IENVELOPEFORMAT_HH
#define PHD_CODE_IENVELOPEFORMAT_HH

#include "IOFormat.hh"
#include "Envelope.hh"

class EnvelopeFormat: public IOFormat<Envelope>{
public:
    virtual void writeEnvelope(const Envelope& obj, std::ostream& output) = 0;

    void writeObject(const Envelope &obj, std::ostream &output) override {
        writeEnvelope(obj, output);
    }


};

#endif //PHD_CODE_IENVELOPEFORMAT_HH
