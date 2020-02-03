//
// Created by zelenyy on 22.01.2020.
//

#ifndef PHD_CODE_DATAMANAGER_HH
#define PHD_CODE_DATAMANAGER_HH

#include <Logger.hh>
#include "DataFileManager.hh"

using namespace std;

struct NumberFile{
    Numbers *number;
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

    void BeginEvent(){
        for (auto item : numbers){
            item.second.number->clear();
        }
    };
    void EndEvent(){
        for (auto item : numbers){
            item.second.number->write(item.second.fout);
        }
    };

private:
    map<string, NumberFile> numbers;
    DataManager()= default;
    DataManager(DataManager const&) = delete;
    DataManager& operator=(DataManager const&) = delete;
};

#endif //PHD_CODE_DATAMANAGER_HH
