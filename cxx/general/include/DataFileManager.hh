//
// Created by mihail on 20.04.17.
//

#ifndef GEANT4_THUNDERSTORM_DATAFILEMANAGER_HH
#define GEANT4_THUNDERSTORM_DATAFILEMANAGER_HH

#include <map>
#include <utility>
#include <string>
#include <sys/stat.h>
#include "DataFile.hh"
#include "FileUtils.hh"
#include "SocketOutput.hh"


using namespace std;

class DataFileManager {

public:
    static DataFileManager *instance() {
        static DataFileManager dataFileManager;
        return &dataFileManager;
    }

    template<class Data>
    DataFile<Data> *getDataFile(const string &name) {
        if (dataFileMap.find(name) == dataFileMap.end()) {
            dataFileMap[name] = new DataFile<Data>(name);
        }
        return (DataFile<Data> *) dataFileMap[name];
    }

    ofstream *getTextFile(const string &name) {
        if (textFileMap.find(name) == textFileMap.end()) {
            string nameFile = checkFileName(name, 0, ".txt");
            auto *fout = new ofstream;
            fout->open(nameFile);
            textFileMap[name] = fout;
        }
        return textFileMap[name];
    }

    ofstream *getBinaryFile(const string &name) {
        if (binaryFileMap.find(name) == binaryFileMap.end()) {
            string nameFile = checkFileName(name, 0, ".bin");
            auto *fout = new ofstream;
            fout->open(nameFile, ios_base::binary | ios_base::out | ios_base::trunc);
            binaryFileMap[name] = fout;
        }
        return binaryFileMap[name];
    }



    ~DataFileManager() {
        for (auto &it : dataFileMap) {
            delete it.second;
        }
        for (auto it : textFileMap){
            it.second->close();
        }
        for (auto it : binaryFileMap){
            it.second->close();
        }
        for (auto it : models){
            cout<<it.first<<" "<<it.second<<endl;
        }
    };
private:
    map<string, IDataFile *> dataFileMap;
    map<string, ofstream *> textFileMap;
    map<string, ofstream *> binaryFileMap;

    DataFileManager() = default;

    DataFileManager(DataFileManager const &);

    DataFileManager &operator=(DataFileManager const &);

public:
    map<string, int> models;
};


#endif //GEANT4_THUNDERSTORM_DATAFILEMANAGER_HH
