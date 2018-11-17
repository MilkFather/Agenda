#include "AgendaService.hpp"
#include <set>
#include "AgendaException.hpp"

AgendaService::AgendaService() {
    startAgenda();
}

AgendaService::~AgendaService() {
    quitAgenda();
}

bool AgendaService::userLogIn(const std::string &userName, const std::string &password) {
    bool fail = m_storage->queryUser([userName, password](const User & x) -> bool {
        return (x.getName() == userName && x.getPassword() == password);
    }).empty();

    if (fail) {
        throw UserPasswordNotMatchException();
        return false;
    } else {
        return true;
    }
}

bool AgendaService::userRegister(const std::string &userName, const std::string &password, const std::string &email, const std::string &phone) {
    if (!m_storage->queryUser([userName](const User &x) -> bool {
        return (x.getName() == userName);
    }).empty()) {
        throw UsernameDuplicateException(userName);
        return false;  // creating existing user name.
    }

    m_storage->createUser(User(userName, password, email, phone));
    return true;
}

bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
    if (m_storage->queryUser([userName, password](const User &x) -> bool {
        return (x.getName() == userName && x.getPassword() == password);
    }).empty()) {
        throw UserPasswordNotMatchException();
        return false;  // incorrect username or password
    }
    
    deleteAllMeetings(userName);
    for (auto meeting: listAllParticipateMeetings(userName))
        quitMeeting(userName, meeting.getTitle());
    
    m_storage->deleteUser([userName](const User &x) -> bool {
        return x.getName() == userName;
    });
    m_storage->deleteMeeting([](const Meeting &x) -> bool {
        return x.getParticipator().empty();
    });
    return true;
}

std::list<User> AgendaService::listAllUsers(void) const {
    return m_storage->queryUser([](const User & x) -> bool {
        return true;
    });
}

bool AgendaService::createMeeting(const std::string &userName, const std::string &title, const std::string &startDate, const std::string &endDate, const std::vector<std::string> &participator) {
    // user basic check
    if (m_storage->queryUser([userName](const User &x) -> bool {
        return x.getName() == userName;
    }).empty()) {
        throw UserNotExistException(userName);
        return false;  // the sponsor does not exist
    }
    
    if (participator.empty()) {
        throw ParticipatorEmptyException(title);
        return false;  // no participators
    }

    std::set<std::string> partpool;
    for (auto n: participator) {
        if (partpool.find(n) != partpool.end()) {
            throw ParticipatorDuplicateException(title, n);
            return false;  // duplicate participator
        }
        if (n == userName) {
            throw SponsorIsParticipatorExcpetion(title, n);
            return false;  // sponsor is participator
        }
        if (m_storage->queryUser([n](const User & x) -> bool {
            return x.getName() == n;
        }).empty()) {
            throw UserNotExistException(n);
            return false;  // participator does not exist
        }
        partpool.insert(n);
    }
    
    // time basic check
    Date sd(startDate), ed(endDate);
    if (!Date::isValid(sd)) {
        throw TimeInvalidException(startDate);
        return false;   // dates are invalid
    }
    if (!Date::isValid(ed)) {
        throw TimeInvalidException(endDate);
        return false;   // dates are invalid
    }
    if (sd >= ed) {
        throw TimeIntervalInvalidException(startDate, endDate);
        return false;  // the start date is not earlier than end date
    }
        
    
    // meeting info basic check
    if (!m_storage->queryMeeting([title](const Meeting & x) -> bool {
        return x.getTitle() == title;
    }).empty()) {
        throw MeetingDuplicateException(title);
        return false;  // duplicate title
    }
    
    // free time check
    if (!m_storage->queryMeeting([userName, sd, ed](const Meeting &x) -> bool {
        return ((x.getSponsor() == userName || x.isParticipator(userName))) && x.getStartDate() < ed && x.getEndDate() > sd;
    }).empty()) {
        throw UserBusyException(title, userName);
        return false;  // sponsor is busy
    }

    for (auto n: participator) {
        if (!m_storage->queryMeeting([n, sd, ed](const Meeting &x) -> bool {
            return (x.getSponsor() == n || x.isParticipator(n)) && x.getStartDate() < ed && x.getEndDate() > sd;
        }).empty()) {
            throw UserBusyException(title, n);
            return false;  // participator is busy
        }
    }
    
    // all done
    m_storage->createMeeting(Meeting(userName, participator, sd, ed, title));
    return true;
}

bool AgendaService::addMeetingParticipator(const std::string &userName, const std::string &title, const std::string &participator) {
    if (m_storage->queryUser([userName](const User &x) -> bool {
        return x.getName() == userName;
    }).empty()) {
        throw UserNotExistException(userName);
        return false;  // the sponsor does not exist
    }
    
    if (m_storage->queryUser([participator](const User &x) -> bool {
        return x.getName() == participator;
    }).empty()) {
        throw UserNotExistException(participator);
        return false;  // the participator does not exist
    }
    
    auto ls = m_storage->queryMeeting([userName, title](const Meeting &x) -> bool {
        return (x.getSponsor() == userName && x.getTitle() == title);
    });
    if (ls.size() != 1) {
        throw MeetingNotExistException(title);
        return false;  // specific meeting not exist or duplicate meetings
    }
    auto m = *(ls.begin());
    if (m.isParticipator(participator)) {
        throw ParticipatorDuplicateException(title, participator);
        return false;  // already in
    }
    
    if (userName == participator) {
        throw SponsorIsParticipatorExcpetion(title, userName);
        return false;  // sponsor is participator
    }
    
    if (m.isParticipator(userName)) {
        throw ParticipatorDuplicateException(title, userName);
        return false;  // username is participator
    }
    
    if (!m_storage->queryMeeting([participator, m](const Meeting &x) -> bool {
        return x.getSponsor() == participator && x.getStartDate() < m.getEndDate() && x.getEndDate() > m.getStartDate();
    }).empty()) {
        throw UserBusyException(title, participator);
        return false;  // the participator sponsors a meeting that has conflict time
    }

    if (!m_storage->queryMeeting([participator, m](const Meeting &x) -> bool {
        return x.isParticipator(participator) && x.getStartDate() < m.getEndDate() && x.getEndDate() > m.getStartDate();
    }).empty()) {
        throw UserBusyException(title, participator);
        return false;  // the participator participates a meeting that has conflict time
    }
    
    // all done, maybe
    int updated = m_storage->updateMeeting([title, userName](const Meeting &x) -> bool {
        return x.getSponsor() == userName && x.getTitle() == title;
    }, [participator](Meeting &x) -> void {
        x.addParticipator(participator);
    });
    if (updated <= 0) {
        throw MeetingNotExistException(title);
        return false;
    } else {
        return true;
    }
}

bool AgendaService::removeMeetingParticipator(const std::string &userName, const std::string &title, const std::string &participator) {
    if (m_storage->queryUser([userName](const User &x) -> bool {
        return x.getName() == userName;
    }).empty()) {
        throw UserNotExistException(userName);
        return false;  // the sponsor does not exist
    }
    
    if (m_storage->queryUser([participator](const User &x) -> bool {
        return x.getName() == participator;
    }).empty()) {
        throw UserNotExistException(participator);
        return false;  // the participator does not exist
    }
    
    auto ls = m_storage->queryMeeting([userName, title](const Meeting &x) -> bool {
        return x.getSponsor() == userName && x.getTitle() == title;
    });
    if (ls.size() != 1) {
        throw MeetingNotExistException(title);
        return false;  // specific meeting not exist or duplicate meetings
    }
    
    if (userName == participator) {
        throw SponsorIsParticipatorExcpetion(title, userName);
        return false;  // sponsor cannot remove itself
    }
    
    auto m = *(ls.begin());
    if (!m.isParticipator(participator)) {
        throw NotAMemberOfMeetingException(title, participator);
        return false;  // not in the meeting
    }
    
    // all done
    int updated = m_storage->updateMeeting([userName, title](const Meeting &x) -> bool {
        return x.getSponsor() == userName && x.getTitle() == title;
    }, [participator](Meeting &x) -> void {
        x.removeParticipator(participator);
    });
    
    m_storage->deleteMeeting([](const Meeting &x) -> bool {
        return x.getParticipator().empty();
    });

    if (updated <= 0) {
        throw MeetingNotExistException(title);
        return false;
    } else {
        return true;
    }
}

bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
    if (m_storage->queryUser([userName](const User &x) -> bool {
        return x.getName() == userName;
    }).empty()) {
        throw UserNotExistException(userName);
        return false;  // the sponsor does not exist
    }
    
    if (m_storage->queryMeeting([userName, title](const Meeting &x) -> bool {
        return (x.getSponsor() != userName && x.isParticipator(userName) && x.getTitle() == title);
    }).empty()) {
        throw MeetingNotExistException(title);
        return false;  // not exist or duplicate
    }
    
    int updated = m_storage->updateMeeting([userName, title](const Meeting &x) -> bool {
        return (x.getSponsor() != userName && x.isParticipator(userName) && x.getTitle() == title);
    }, [userName](Meeting &x) -> void {
        x.removeParticipator(userName);
    });
    
    m_storage->deleteMeeting([](const Meeting &x) -> bool {
        return x.getParticipator().empty();
    });

    if (updated <= 0) {
        throw MeetingNotExistException(title);
        return false;
    } else {
        return true;
    }
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName, const std::string &title) const {
    std::list<Meeting> m;
    if (m_storage->queryUser([userName](const User &x) -> bool {
        return x.getName() == userName;
    }).empty()) {
        throw UserNotExistException(userName);
        return m;  // the user does not exist, returning empty list
    }
    
    m = m_storage->queryMeeting([userName, title](const Meeting &x) -> bool {
        return x.getTitle() == title && (x.getSponsor() == userName || x.isParticipator(userName));
    });
    
    return m;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName, const std::string &startDate, const std::string &endDate) const {
    Date sd(startDate), ed(endDate);
    std::list<Meeting> ml;
    
    if (m_storage->queryUser([userName](const User &x) -> bool {
        return x.getName() == userName;
    }).empty()) {
        throw UserNotExistException(userName);
        return ml;  // the user does not exist, returning empty list
    }
    
    if (!Date::isValid(sd)) {
        throw TimeInvalidException(startDate);
        return ml;  // query date invalid
    }
    if (!Date::isValid(ed)) {
        throw TimeInvalidException(endDate);
        return ml;  // query date invalid
    }
    if (sd > ed) {
        throw TimeIntervalInvalidException(startDate, endDate);
        return ml;  // query date invalid
    }
    
    return m_storage->queryMeeting([userName, sd, ed](const Meeting &x) -> bool {
        return (x.getSponsor() == userName || x.isParticipator(userName)) && x.getEndDate() >= sd && x.getStartDate() <= ed;
    });
}

std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
    std::list<Meeting> m;
    if (m_storage->queryUser([userName](const User &x) -> bool {
        return x.getName() == userName;
    }).empty()) {
        throw UserNotExistException(userName);
        return m;  // the user does not exist, returning empty list
    }
    
    return m_storage->queryMeeting([userName](const Meeting &x) -> bool {
        return x.getSponsor() == userName || x.isParticipator(userName);
    });
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
    std::list<Meeting> m;
    if (m_storage->queryUser([userName](const User &x) -> bool {
        return x.getName() == userName;
    }).empty()) {
        throw UserNotExistException(userName);
        return m;  // the user does not exist, returning empty list
    }
    
    return m_storage->queryMeeting([userName](const Meeting &x) -> bool {
        return x.getSponsor() == userName;
    });
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string &userName) const {
    std::list<Meeting> m;
    if (m_storage->queryUser([userName](const User &x) -> bool {
        return x.getName() == userName;
    }).empty()) {
        throw UserNotExistException(userName);
        return m;  // the user does not exist, returning empty list
    }
    
    return m_storage->queryMeeting([userName](const Meeting &x) -> bool {
        return x.isParticipator(userName);
    });
}

bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
    if (m_storage->queryUser([userName](const User &x) -> bool {
        return x.getName() == userName;
    }).empty()) {
        throw UserNotExistException(userName);
        return false;  // the user does not exist
    }
    
    int deleted = m_storage->deleteMeeting([userName, title](const Meeting &x) -> bool {
        return (x.getSponsor() == userName) && (x.getTitle() == title);
    });
    if (deleted <= 0) {
        throw MeetingNotExistException(title);
        return false;
    } else {
        return true;
    }
}

bool AgendaService::deleteAllMeetings(const std::string &userName) {
    if (m_storage->queryUser([userName](const User &x) -> bool {
        return x.getName() == userName;
    }).empty()) {
        throw UserNotExistException(userName);
        return false;  // the user does not exist
    }
    
    m_storage->deleteMeeting([userName](const Meeting &x) -> bool {
        return x.getSponsor() == userName;
    });
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
