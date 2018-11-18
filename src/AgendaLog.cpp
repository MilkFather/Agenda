#include "AgendaLog.hpp"
#include <dirent.h>

shared_ptr<AgendaLogMan> AgendaLogMan::m_instance(nullptr);

AgendaLogMan::AgendaLogMan() {
    /* todo */
}

AgendaLogMan::~AgendaLogMan() {
    /* todo */
}

shared_ptr<AgendaLogMan> AgendaLogMan::getInstance() {
    if (AgendaLogMan::m_instance == nullptr) {
        AgendaLogMan::m_instance = std::shared_ptr<AgendaLogMan>(new AgendaLogMan());
    }
    return AgendaLogMan::m_instance;
}