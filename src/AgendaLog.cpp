#include "AgendaLog.hpp"
#include <exception>
#include <ctime>
#include <cstdlib>
#include <sstream>

using std::exception;
using std::stringstream;
using std::ios_base;
using std::shared_ptr;
using std::ifstream;
using std::endl;

shared_ptr<AgendaLogMan> AgendaLogMan::m_instance(nullptr);

AgendaLogMan::AgendaLogMan() {
#ifdef _WIN32
    string f = string(Path::logDirPathWin) + string("Agenda.log");
    string cmd = string("if not exist \"") + string(Path::logDirPathWin) + string("\" mkdir \"") + string(Path::logDirPathWin) + string("\"");
#else
    string f = string(Path::logDirPath) + string("Agenda.log");
    string cmd = string("mkdir -p \"") + string(Path::logDirPath) + string("\"");
#endif
    system(cmd.c_str());
    size = getFileSize(f);
    handle.open(f, ios_base::app);
    Log("AgendaLogManager: Log file open.");
}

AgendaLogMan::~AgendaLogMan() {
    Log("AgendaLogManager: Log file closed.");
    handle.close();
}

shared_ptr<AgendaLogMan> AgendaLogMan::getInstance() {
    if (AgendaLogMan::m_instance == nullptr) {
        AgendaLogMan::m_instance = shared_ptr<AgendaLogMan>(new AgendaLogMan());
    }
    return AgendaLogMan::m_instance;
}

int AgendaLogMan::getFileSize(const string file) const {
    ifstream in(file, ifstream::ate | ifstream::binary);
    return in.tellg();
}

void AgendaLogMan::Log(const string l) {
    if (handle.is_open()) {
        time_t rawtime;
        struct tm * timeinfo;
        char buffer[80];

        time (&rawtime);
        timeinfo = localtime(&rawtime);

        strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
        string str(buffer);

        str += string(" " + l);

        handle << str << endl;
        
        size += str.length();
        if (size >= 1024 * 10) {    // 10KB
            stringstream ss;
            ss << "Agenda_" 
               << timeinfo->tm_year + 1900 
               << "-" << timeinfo->tm_mon + 1 
               << "-" << timeinfo->tm_mday
               << "-" << timeinfo->tm_hour
               << "-" << timeinfo->tm_min
               << "-" << timeinfo->tm_sec
               << ".log";
            handle.close();
            string f = string(Path::logDirPath) + string("Agenda.log");
#ifdef _WIN32
            string cmd = string("ren \"") + f + string("\" \"") + ss.str() + string("\"");
#else
            string cmd = string("mv \"") + f + string("\" \"") + string(Path::logDirPath) + ss.str() + string("\"");
#endif
            system(cmd.c_str());
            handle.open(f, ios_base::app);
            
            size = 0;
        }
    }
}