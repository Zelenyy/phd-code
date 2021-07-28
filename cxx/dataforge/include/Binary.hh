//
// Created by zelenyy on 24.02.2020.
//

#ifndef PHD_CODE_BINARY_HH
#define PHD_CODE_BINARY_HH

#include <cstddef>
#include <istream>
#include <functional>

class Binary{
public:
    std::size_t getSize(){
        return size;
    }

    virtual bool read(std::function<bool(std::istream&)> input) = 0;

    virtual ~Binary() = default;
protected:
    std::size_t size{};
};

#endif //PHD_CODE_BINARY_HH
