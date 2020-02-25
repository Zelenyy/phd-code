//
// Created by zelenyy on 24.02.2020.
//

#ifndef PHD_CODE_IENVELOPE_HH
#define PHD_CODE_IENVELOPE_HH

#include "Binary.hh"


class Envelope{
public:
//    Meta& meta
 Binary* binary;
virtual ~Envelope() = default;

};

#endif //PHD_CODE_IENVELOPE_HH
