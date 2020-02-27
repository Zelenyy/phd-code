//
// Created by zelenyy on 24.02.2020.
//

#ifndef PHD_CODE_ARRAYBINARY_HH
#define PHD_CODE_ARRAYBINARY_HH


#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include "RandomAccessBinary.hh"

template <typename T>
class ArrayBinary : public RandomAccessBinary {
public:
    const std::vector<T>& array;

    explicit ArrayBinary(const std::vector<T>& array);

    bool read(std::function<bool(std::istream&)> input, unsigned int from, unsigned int size) override;


};


template<typename T>
ArrayBinary<T>::ArrayBinary(const std::vector<T> &array)  : array(array){
    this->size = sizeof(T)*array.size();
}

template<typename T>
bool ArrayBinary<T>::read(std::function<bool(std::istream&)> input, unsigned int from, unsigned int size) {
    size_t theSize = (size < this->size - from) ? size : this->size - from;
    std::stringstream temp;
    char* front = (char*) &array.front();
    front+=from;
    temp.write(front, theSize);
    return input(temp);
}





#endif //PHD_CODE_ARRAYBINARY_HH
