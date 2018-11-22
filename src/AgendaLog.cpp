#include "AgendaLog.hpp"
#include <exception>

using std::exception;

shared_ptr<AgendaLogMan> AgendaLogMan::m_instance(nullptr);

AgendaLogMan::AgendaLogMan() {
    string f = Path::logDirPath;
    f += "Agenda.log";
    handle.open(f, std::ios_base::app);
}

AgendaLogMan::~AgendaLogMan() {
    handle.close();
}

shared_ptr<AgendaLogMan> AgendaLogMan::getInstance() {
    if (AgendaLogMan::m_instance == nullptr) {
        AgendaLogMan::m_instance = std::shared_ptr<AgendaLogMan>(new AgendaLogMan());
    }
    return AgendaLogMan::m_instance;
}

void AgendaLogMan::Log(string l) {
    if (handle.is_open()) {
        handle << l << std::endl;
    } else {
        throw exception();
    }

}