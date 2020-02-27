//
// Created by zelenyy on 25.02.2020.
//

#ifndef PHD_CODE_DATACELL_HH
#define PHD_CODE_DATACELL_HH

#include <utility>
#include <string>
#include "ArrayBinary.hh"
#include "SimpleEnvelope.hh"

class DataStorage;

using namespace std;

class IDataCell{
protected:
    string name;
    DataStorage* dataStorage;
public:
    IDataCell(string  name, DataStorage* dataStorage): name(std::move(name)), dataStorage(dataStorage){};

    virtual void dropData() = 0;
    virtual ~IDataCell() = default;
};

template <class T, int size_step = 1000>
class MonolithDataCell: public IDataCell{
private:
    vector<T> dataArray;
    int size = size_step;
    int indx = 0;
public:
    MonolithDataCell(string name, DataStorage* dataStorage) : IDataCell(name, dataStorage){
      dataArray.reserve(size_step);
    };
    void addData(T& data){
        dataArray[indx] = data;
        indx++;
        if (indx == size){
            dataArray.resize(size + size_step);
            size += size_step;
        }
        };

    void dropData() override{
        dataArray.resize(indx);
        ArrayBinary<T> binary(dataArray);
        SimpleEnvelope envelope(binary);
        dataStorage->writeEnvelope(name, envelope);

        indx = 0;
        dataArray.resize(size_step);
        dataArray.shrink_to_fit();

    }
};

template <class T, int buff_size=1000>
class BufferedDataCell : public  IDataCell{
private:
    vector<T> dataArray;
    int indx = 0;
public:
    BufferedDataCell(string name, DataStorage* dataStorage) : IDataCell(name, dataStorage){
        dataArray.reserve(buff_size);
        dataArray.resize(buff_size);
    };
    void addData(T& data){
        dataArray[indx] = data;
        indx++;
        if (indx == buff_size){
            dataArray.resize(buff_size);
            ArrayBinary<T> binary(dataArray);
            SimpleEnvelope envelope(binary);
            dataStorage->writeEnvelope(name, envelope);
            indx = 0;
        }
    }

    void dropData() override{
        dataArray.resize(indx);
        ArrayBinary<T> binary(dataArray);
        SimpleEnvelope envelope(binary);
        dataStorage->writeEnvelope(name, envelope);
        indx = 0;
        dataArray.resize(buff_size);

    }
};

#endif //PHD_CODE_DATACELL_HH
