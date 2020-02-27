//
// Created by zelenyy on 24.02.2020.
//

#ifndef PHD_CODE_RANDOMACCESSBINARY_HH
#define PHD_CODE_RANDOMACCESSBINARY_HH

#include <climits>
#include "Binary.hh"


class RandomAccessBinary: public Binary{
public:
    virtual bool read(std::function<bool(std::istream&)> input, unsigned int from, unsigned int size) = 0;

    bool read(std::function<bool(std::istream&)> input) override {
        return read(input, 0, UINT_MAX);
    }

};

#endif //PHD_CODE_RANDOMACCESSBINARY_HH
