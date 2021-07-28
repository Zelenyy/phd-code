//
// Created by zelenyy on 26.02.2020.
//

#include <sstream>
#include <sys/socket.h>
#include "TaggedEnvelopeFormat.hh"


TaggedEnvelopeFormat::TaggedEnvelopeFormat(TaggedEnvelopeFormatVersion version) : version(version) {

}


void TaggedEnvelopeFormat::writeEnvelope(const Envelope &obj, std::ostream &output) {
    std::string header = getHeader(0, 0, obj.binary->getSize());
    output<<header;
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


std::string TaggedEnvelopeFormat::getHeader(short metaFormatKey, unsigned int metaSize, size_t dataSize) {
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
    std::string header = "";
    std::stringstream temp("", std::ios::binary);
    temp << START_SEQUENCE;
    temp << name;
    temp << metaFormatKey;
    temp << metaSize;
    switch (version) {
        case TaggedEnvelopeFormatVersion::DF02:
            temp << static_cast<unsigned int>(dataSize);
            break;
        case TaggedEnvelopeFormatVersion::DF03:
            temp << dataSize;
            break;
    }
    temp << END_SEQUENCE;
    temp.flush();
    temp >> header;
    return header;
}

void TaggedEnvelopeFormat::writeEnvelope(const Envelope &obj, int fileDescriptor) {
    std::string header = getHeader(0, 0, obj.binary->getSize());
    send(fileDescriptor, header.c_str(), header.length(), 0);

}
