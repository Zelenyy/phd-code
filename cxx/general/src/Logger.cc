//
// Created by zelenyy on 25.07.17.
//

#include <DataFileManager.hh>
#include "Logger.hh"

using namespace std;

Logger::~Logger() {
    time_t secondsEnd = time(NULL);
    *flog << getTimeString() << " simulation end" <<endl;
    *flog << getTimeString() << " time of simulation in seconds: " << secondsEnd - seconds << endl;
    *flog << getTimeString() << " The end." << endl;
}

void Logger::print(string message) {
    *flog<< getTimeString() <<" " << message << endl << flush;
}

Logger::Logger() {
    flog = DataFileManager::instance()->getTextFile("log");
    seconds = time(NULL);
    *flog << getTimeString()<<" "<< "simulation start."<<endl<<flush;
}

string Logger::getTimeString() {
    time_t secondsTemp = time(NULL);
    tm *timeinfo = localtime(&secondsTemp);
    string time(asctime(timeinfo));
    time.pop_back();
    return "[" + time + "]" ;
}
