//
// Created by zelenyy on 22.01.2020.
//

#ifndef PHD_CODE_DATAMANAGER_HH
#define PHD_CODE_DATAMANAGER_HH

#include <Logger.hh>
#include "DataFileManager.hh"
#include "Data.hh"

using namespace std;

struct NumberFile{
    Numbers *number;
    ofstream * fout;
};

struct NamedNumberFile{
    NamedNumbers *number;
    ofstream * fout;
};

class DataManager{
public:
    static DataManager* instance(){
        static DataManager dataManager;
        return &dataManager;
    }

    Numbers* createNumber(const string& name){
        auto * data = new Numbers;
        ofstream* foutNumber = DataFileManager::instance()->getTextFile(name);
        NumberFile temp = {data, foutNumber};
        if (numbers.find(name)==numbers.end()) {
            numbers[name] = temp;
            Logger::instance()->print("Register number file: " + name);
        }
        return data;
    }

    NamedNumbers* createNamedNumber(const string& name){
        auto * data = new NamedNumbers;
        ofstream* foutNumber = DataFileManager::instance()->getTextFile(name);
        NamedNumberFile temp = {data, foutNumber};
        if (namedNumbers.find(name)==namedNumbers.end()) {
            namedNumbers[name] = temp;
            Logger::instance()->print("Register named number file: " + name);
        }
        return data;
    }

    void BeginEvent(){
        for (auto item : numbers){
            item.second.number->clear();
        }
        for (auto item : namedNumbers){
            item.second.number->clear();
        }
    };
    void EndEvent(){
        for (auto item : numbers){
            item.second.number->write(item.second.fout);
        }
        for (auto item : namedNumbers){
            item.second.number->write(item.second.fout);
        }
    };

private:
    map<string, NumberFile> numbers;
    map<string, NamedNumberFile> namedNumbers;
    DataManager()= default;
    DataManager(DataManager const&) = delete;
    DataManager& operator=(DataManager const&) = delete;
};

#endif //PHD_CODE_DATAMANAGER_HH
