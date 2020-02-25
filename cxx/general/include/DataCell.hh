//
// Created by zelenyy on 25.02.2020.
//

#ifndef PHD_CODE_DATACELL_HH
#define PHD_CODE_DATACELL_HH

#include "ArrayBinary.hh"

class DataStorage;

class IDataCell{
public:
    virtual ~IDataCell() = default;
};

template <class T, int buff_size>
class DataCell : public  IDataCell{
private:
    DataStorage* dataStorage;
    vector<T> dataArray;
    string name;
    int indx = 0;
public:
    explicit DataCell(string name, DataStorage* dataStorage) : name(name), dataStorage(dataStorage){
        dataArray.reserve(buff_size);
    };

    void addData(T& data){
        dataArray[indx] = data;
        indx++;
        if (indx == buff_size){
            ArrayBinary<T> temp(data)
            dataStorage.writePackege(name, )
            indx = 0
        }
    }
};

#endif //PHD_CODE_DATACELL_HH
