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




TaggedEnvelopeFormat::TaggedEnvelopeFormat(TaggedEnvelopeFormatVersion version) : version(version) {

}


void TaggedEnvelopeFormat::writeEnvelope(const Envelope &obj, std::ostream &output) {
    writeTag(output, 0, 0, obj.binary->getSize());
    //writeMeta
    output << "\r\n";
    if (obj.binary != nullptr) {
        obj.binary->read(
                [&output, &obj](std::istream& input){
                    output<<input.rdbuf();
                    return true;
                }
        );
    }
    output.flush();
}


void TaggedEnvelopeFormat::writeTag(std::ostream &output, short metaFormatKey, unsigned int metaSize, size_t dataSize) {
    int tagSize;
    std::string name;
    switch (version) {
        case TaggedEnvelopeFormatVersion::DF02:
            tagSize = 20;
            name = "DF02";
            break;
        case TaggedEnvelopeFormatVersion::DF03:
            tagSize = 24;
            name = "DF03";
            break;
    }
    char header[tagSize];
    output << START_SEQUENCE;
    output << name;
    output << metaFormatKey;
    output << metaSize;
    switch (version) {
        case TaggedEnvelopeFormatVersion::DF02:
            output << static_cast<unsigned int>(dataSize);
            break;
        case TaggedEnvelopeFormatVersion::DF03:
            output << dataSize;
            break;
    }
    output << END_SEQUENCE;

}



#endif //PHD_CODE_TAGGEDENVELOPEFORMAT_HH
