#include "AgendaService.hpp"
#include <set>

AgendaService::AgendaService() {
    this->startAgenda();
}

AgendaService::~AgendaService() {
    this->quitAgenda();
}

bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
    return (!m_storage->queryUser([userName, password](const User & x) -> bool {return (x.getName() == userName && x.getPassword() == password);}).empty());
}

bool AgendaService::userRegister(const std::string &userName, const std::string &password, const std::string &email, const std::string &phone) {
    if (!m_storage->queryUser([userName](const User &x) -> bool {return (x.getName() == userName);}).empty())
        return false;  // creating existing user name.
    m_storage->createUser(User(userName, password, email, phone));
    return true;
}

bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
    if (m_storage->queryUser([userName, password](const User &x) -> bool {return (x.getName() == userName && x.getPassword() == password);}).empty())
        return false;  // incorrect username or password
    deleteAllMeetings(userName);
    for (auto meeting: listAllParticipateMeetings(userName))
        quitMeeting(userName, meeting.getTitle());
    m_storage->deleteUser([userName](const User &x) -> bool {return x.getName() == userName;});
    return true;
}

std::list<User> AgendaService::listAllUsers(void) const {
    return m_storage->queryUser([](const User & x) -> bool {return true;});
}

bool AgendaService::createMeeting(const std::string &userName, const std::string &title, const std::string &startDate, const std::string &endDate, const std::vector<std::string> &participator) {
    // user basic check
    if (m_storage->queryUser([userName](const User &x) {return x.getName() == userName;}).empty())
        return false;  // the sponsor does not exist
    if (participator.empty())
        return false;  // no participators
    std::set<std::string> partpool;
    for (auto n: participator) {
        if (partpool.find(n) != partpool.end())
            return false;  // duplicate participator
        if (n == userName)
            return false;  // sponsor is participator
        if (m_storage->queryUser([n](const User & x) {return x.getName() == n;}).empty())
            return false;  // participator does not exist
        partpool.insert(n);
    }
    
    // time basic check
    Date sd(startDate), ed(endDate);
    if (!Date::isValid(sd) || !Date::isValid(ed) || sd >= ed)
        return false;  // dates are invalid, or the start date is not earlier than end date
    
    // meeting info basic check
    if (!m_storage->queryMeeting([title](const Meeting & x) {return x.getTitle() == title;}).empty())
        return false;  // duplicate title
    
    // free time check
    if (!m_storage->queryMeeting([userName, sd, ed](const Meeting &x) {return x.getSponsor() == userName && x.getStartDate() < ed && x.getEndDate() > sd;}).empty())
        return false;  // sponsor is busy
    for (auto n: participator) {
        if (!m_storage->queryMeeting([n, sd, ed](const Meeting &x) {return x.getSponsor() == n && x.getStartDate() < ed && x.getEndDate() > sd;}).empty())
            return false;  // participator is busy
    }
    
    // all done
    m_storage->createMeeting(Meeting(userName, participator, sd, ed, title));
    return true;
}

bool AgendaService::addMeetingParticipator(const std::string &userName, const std::string &title, const std::string &participator) {
    if (userName == participator)
        return false;  // sponsor is participator
    auto ls = m_storage->queryMeeting([userName, title](const Meeting &x) {return (x.getSponsor() == userName && x.getTitle() == title);});
    if (ls.size() != 1)
        return false;  // specific meeting not exist or duplicate meetings
    auto m = *(ls.begin());
    if (m.isParticipator(participator))
        return false;  // already in

    if (!this->m_storage.get()->queryMeeting([participator, m](const Meeting &x) {return x.getSponsor() == participator && x.getStartDate() < m.getEndDate() && x.getEndDate() > m.getStartDate();}).empty())
        return false;  // the participator sponsors a meeting that has conflict time
    if (!this->m_storage.get()->queryMeeting([participator, m](const Meeting &x) {return x.isParticipator(participator) && x.getStartDate() < m.getEndDate() && x.getEndDate() > m.getStartDate();}).empty())
        return false;  // the participator participates a meeting that has conflict time
        
    // all done
    m_storage->updateMeeting([m](const Meeting &x) {return x.getTitle() == m.getTitle();}, [participator](Meeting &x) {x.addParticipator(participator);});
    return true;
}

bool AgendaService::removeMeetingParticipator(const std::string &userName, const std::string &title, const std::string &participator) {
    if (userName == participator)
        return false;  // sponsor cannot remove itself
    auto ls = m_storage->queryMeeting([userName, title](const Meeting &x) {return x.getSponsor() == userName && x.getTitle() == title;});
    if (ls.size() != 1)
        return false;  // specific meeting not exist or duplicate meetings
    auto m = *(ls.begin());
    if (!m.isParticipator(participator))
        return false;  // not in the meeting
    
    // all done
    m_storage->updateMeeting([title](const Meeting &x) {return x.getTitle() == title;}, [participator](Meeting &x) {x.removeParticipator(participator);});
    return true;
}

bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
    if (m_storage->queryMeeting([userName, title](const Meeting &x) {return (x.getSponsor() != userName && x.isParticipator(userName) && x.getTitle() == title);}).size() != 1)
        return false;  // not exist or duplicate
    
    m_storage->updateMeeting([userName, title](const Meeting &x) {return (x.getSponsor() != userName && x.isParticipator(userName) && x.getTitle() == title);}, [userName](Meeting &x) {x.removeParticipator(userName);});
    m_storage->deleteMeeting([](const Meeting &x) {return x.getParticipator().empty();});
    return true;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName, const std::string &title) const {
    return m_storage->queryMeeting([userName, title](const Meeting &x) {return x.getTitle() == title && (x.getSponsor() == userName || x.isParticipator(userName));});
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName, const std::string &startDate, const std::string &endDate) const {
    Date sd(startDate), ed(startDate);
    std::list<Meeting> ml;
    if (!Date::isValid(sd) || Date::isValid(ed))
        return ml;  // query date invalid
    
    return m_storage->queryMeeting([userName, sd, ed](const Meeting &x) {return (x.getSponsor() == userName || x.isParticipator(userName)) && x.getEndDate() >= sd && x.getStartDate() <= ed;});
}

std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
    return this->m_storage->queryMeeting([userName](const Meeting &x) {return x.getSponsor() == userName || x.isParticipator(userName);});
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
    return m_storage->queryMeeting([userName](const Meeting &x) {return x.getSponsor() == userName;});
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string &userName) const {
    return m_storage->queryMeeting([userName](const Meeting &x) {return x.isParticipator(userName);});
}

bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
    m_storage->deleteMeeting([userName, title](const Meeting &x) {return (x.getSponsor() == userName) && (x.getTitle() == title);});
    return true;
}

bool AgendaService::deleteAllMeetings(const std::string &userName) {
    m_storage->deleteMeeting([userName](const Meeting &x) {return x.getSponsor() == userName;});
    return true;
}

void AgendaService::startAgenda(void) {
    m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
    if (m_storage != NULL) {
        m_storage->sync();
        m_storage = NULL;
    }
}
