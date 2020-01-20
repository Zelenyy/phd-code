//
// Created by zelenyy on 25.07.17.
//

#ifndef GEANT4_SUPPORT_CXX_LOGGER_HH
#define GEANT4_SUPPORT_CXX_LOGGER_HH

#include <ctime>
#include <fstream>
#include <string>

using namespace std;

class Logger {
public:

    static Logger* instance(){
        static Logger logger;
        return  &logger;
    }
    ~Logger();

    void print(string message);

    string getTimeString();

private:
    Logger();
    Logger(Logger const&);
    Logger& operator=(Logger const&);
    ofstream* flog;
    time_t seconds;

};


#endif //GEANT4_SUPPORT_CXX_LOGGER_HH
