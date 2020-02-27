//
// Created by zelenyy on 25.02.2020.
//

#ifndef PHD_CODE_DATASTORAGE_HH
#define PHD_CODE_DATASTORAGE_HH

#include "DataCell.hh"
#include "Envelope.hh"
#include "TaggedEnvelopeFormat.hh"
#include "FileUtils.hh"
#include <map>
using namespace std;
class DataStorage{
private:
    TaggedEnvelopeFormat* format;
    map<string, ostream*> outputs;
    map<string, IDataCell*> dataCell;
public:
    void writeEnvelope(string name, Envelope& envelope){
        if (outputs.find(name) == outputs.end()){
            return;
        }
        format->writeEnvelope(envelope, *(outputs[name]));
    };

    template <class T, int step_size=1000>
    MonolithDataCell<T,step_size>* getToFileMonolithDataCell(const string &name){

        if (outputs.find(name) == outputs.end()) {
            string nameFile = checkFileName(name, 0, ".df");
            auto *fout = new ofstream;
            fout->open(nameFile);
            outputs[name] = fout;
            auto* datacell = new MonolithDataCell<T,step_size>(name, this);
            dataCell[name] =  datacell;
        }
        return (MonolithDataCell<T,step_size>*) dataCell[name];
    };

    template <class T, int step_size=1000>
    MonolithDataCell<T,step_size>* getToSocketMonolithDataCell(const string &name) {
        if (outputs.find(name) == outputs.end()) {
//            __gnu_cxx::stdio_filebuf
            auto *fout = new ofstream;
            auto* datacell = new MonolithDataCell<T,step_size>(name, this);
            dataCell[name] =  datacell;
        }
        return (MonolithDataCell<T,step_size>*) dataCell[name];
    }

    void beginRun(){

    }

    void endRun(){
        for (auto it: dataCell){
            it.second->dropData();
        }
    }
    static DataStorage *instance() {
        static DataStorage dataStorage;
        dataStorage.format = new TaggedEnvelopeFormat;
        return &dataStorage;
    }
private:
    DataStorage() = default;
    DataStorage(DataStorage const&) = delete;
    DataStorage &operator=(DataStorage const &) = delete;
};


#endif //PHD_CODE_DATASTORAGE_HH
