//
// Created by mihail on 20.04.17.
//

#ifndef GEANT4_THUNDERSTORM_DATAFILE_HH
#define GEANT4_THUNDERSTORM_DATAFILE_HH

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include <sys/stat.h>
#include "FileUtils.hh"

using namespace std;





class IDataFile {
public:
    virtual ~IDataFile() = default;
};

template<class Data>
class DataFile : public IDataFile {
public:

    DataFile(string fileName) {
        fileName = checkFileName(fileName, 0);
        fout.open(fileName + ".bin", ios_base::binary | ios_base::out | ios_base::trunc);
        maxSizeData = 1024 * 1024;
        indxData = 0;
        dataArray.reserve(maxSizeData);

    }

    ~DataFile() override {
        writeData();
        fout.close();
    }

    void addData(Data &data) {
        dataArray[indxData] = data;
        indxData++;
        if (indxData == maxSizeData) {
            writeData();
            indxData = 0;
        }
    }

private:
    ofstream fout;
    vector<Data> dataArray;
    int indxData;
    int maxSizeData;

    void writeData() {
        char *filePointer = (char *) &dataArray.front();
        fout.write(filePointer, (sizeof dataArray[0]) * indxData);
        fout.flush();

    }


};
#endif //GEANT4_THUNDERSTORM_DATAFILE_HH
