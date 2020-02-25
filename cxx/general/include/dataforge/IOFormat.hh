//
// Created by zelenyy on 24.02.2020.
//

#ifndef PHD_CODE_IOFORMAT_HH
#define PHD_CODE_IOFORMAT_HH

#include <ostream>

template <typename T>
class IOFormat{
public:
    virtual void writeObject(const T& obj, std::ostream& output) = 0;
//    virtual T& readObject(istream& input) - 0;

};

#endif //PHD_CODE_IOFORMAT_HH
