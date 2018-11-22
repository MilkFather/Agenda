#ifndef AGENDALOG_H
#define AGENDALOG_H

#include "Path.hpp"
#include <string>
#include <memory>
#include <fstream>

using std::string;
using std::shared_ptr;
using std::fstream;

class AgendaLogMan {
private:
    AgendaLogMan();

    AgendaLogMan(const AgendaLogMan &) = delete;
    AgendaLogMan& operator=(const AgendaLogMan &) = delete;

public:
    ~AgendaLogMan();
    static shared_ptr<AgendaLogMan> getInstance();
    void Log(const string l);

private:
    int getFileSize(const string file) const;
    int size;
    static shared_ptr<AgendaLogMan> m_instance;
    fstream handle;
};

#endif