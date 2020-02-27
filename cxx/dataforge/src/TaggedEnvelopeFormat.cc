//
// Created by zelenyy on 26.02.2020.
//

#include "TaggedEnvelopeFormat.hh"


TaggedEnvelopeFormat::TaggedEnvelopeFormat(TaggedEnvelopeFormatVersion version) : version(version) {

}


void TaggedEnvelopeFormat::writeEnvelope(const Envelope &obj, std::ostream &output) {
    writeTag(output, 0, 0, obj.binary->getSize());
    //writeMeta
    output << "\r\n";
    if (obj.binary != nullptr) {
        obj.binary->read(
                [&output](std::istream& input){
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
