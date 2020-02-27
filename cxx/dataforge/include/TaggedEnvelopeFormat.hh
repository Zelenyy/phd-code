//
// Created by zelenyy on 24.02.2020.
//

#ifndef PHD_CODE_TAGGEDENVELOPEFORMAT_HH
#define PHD_CODE_TAGGEDENVELOPEFORMAT_HH

#include "EnvelopeFormat.hh"
#include <string>
#include <algorithm>

enum class TaggedEnvelopeFormatVersion{
    DF02,
    DF03
};



class TaggedEnvelopeFormat : public EnvelopeFormat{

public:
       explicit TaggedEnvelopeFormat(TaggedEnvelopeFormatVersion version = TaggedEnvelopeFormatVersion::DF02);

    void writeEnvelope(const Envelope &obj, std::ostream &output) override;

    const std::string START_SEQUENCE = "#~";
    const std::string END_SEQUENCE = "~#\r\n";
private:
    TaggedEnvelopeFormatVersion version;

    void writeTag(std::ostream &output, short metaFormatKey, unsigned int metaSize, size_t dataSize);


};




#endif //PHD_CODE_TAGGEDENVELOPEFORMAT_HH
