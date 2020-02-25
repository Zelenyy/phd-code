//
// Created by zelenyy on 24.02.2020.
//

#include "SimpleEnvelope.hh"
#include "ArrayBinary.hh"
#include "TaggedEnvelopeFormat.hh"
#include <iostream>
#include <vector>

int main(){

    std::vector<double> data;
    data.reserve(100);
for (int i=0; i<100; ++i){
        data.push_back(i);
    }
    ArrayBinary<double> binary(data);
    std::cout<<binary.getSize()<<std::endl;
    auto envelope = SimpleEnvelope(binary);
//
    auto format = new TaggedEnvelopeFormat();

    format->writeEnvelope(envelope, std::cout);
    return 0;
}