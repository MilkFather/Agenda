#ifndef AGENDALOG_H
#define AGENDALOG_H

#include "Path.hpp"
#include <string>
#include <memory>

using std::string;
using std::shared_ptr;

class AgendaLogMan {
private:
    AgendaLogMan();
    ~AgendaLogMan();

    AgendaLogMan(const AgendaLogMan &) = delete;
    AgendaLogMan& operator=(const AgendaLogMan &) = delete;

    void Openfile();

public:
    static shared_ptr<AgendaLogMan> getInstance();
    void Log(string l);

private:
    static shared_ptr<AgendaLogMan> m_instance;
};

#endif