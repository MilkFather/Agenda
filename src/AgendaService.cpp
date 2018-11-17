#include "AgendaService.hpp"
#include <set>
#include "AgendaException.hpp"

AgendaService::AgendaService() {
    startAgenda();
}

AgendaService::~AgendaService() {
    quitAgenda();
}

void AgendaService::checkUserExistence(const string userName) const {
    if (m_storage->queryUser([userName](const User &x) -> bool {
        return (x.getName() == userName);
    }).empty()) {
        throw UserNotExistException(userName);          // throw when not exist
    }
}

void AgendaService::checkTimeFormat(const string t) const {
    if (!Date::isValid(Date(t))) {
        throw TimeInvalidException(t);                  // dates are invalid
    }
}

void AgendaService::checkUserBusy(const string usr, const Date sd, const Date ed) const {
    if (!m_storage->queryMeeting([usr, sd, ed](const Meeting &x) -> bool {
        return ((x.getSponsor() == usr || x.isParticipator(usr))) && x.getStartDate() < ed && x.getEndDate() > sd;
    }).empty()) {
        throw UserBusyException(usr, Date::dateToString(sd), Date::dateToString(ed));                   // is busy
    }
}

Meeting AgendaService::getMeetingBySponsorAndTitle(const string sponsor, const string title) const {
    auto ls = m_storage->queryMeeting([title](const Meeting &x) -> bool {
        return x.getTitle() == title;
    });
    if (ls.size() != 1) {
        throw MeetingNotExistException(title);
    }
    auto m = *ls.begin();
    if (m.getSponsor() != sponsor) {
        throw MeetingAccessDeniedExcpetion(title, sponsor);
    }
    return m;
}

void AgendaService::userLogIn(const std::string &userName, const std::string &password) {
    if (m_storage->queryUser([userName, password](const User & x) -> bool {
        return (x.getName() == userName && x.getPassword() == password);
    }).empty()) {
        throw UserPasswordNotMatchException();
    }
}

void AgendaService::userRegister(const std::string &userName, const std::string &password, const std::string &email, const std::string &phone) {
    if (!m_storage->queryUser([userName](const User &x) -> bool {
        return (x.getName() == userName);
    }).empty()) {
        throw UsernameDuplicateException(userName);     // creating existing user name.
    }

    m_storage->createUser(User(userName, password, email, phone));
}

void AgendaService::deleteUser(const std::string &userName, const std::string &password) {
    userLogIn(userName, password);                      // the correct user & password
    
    deleteAllMeetings(userName);
    for (auto meeting: listAllParticipateMeetings(userName))
        quitMeeting(userName, meeting.getTitle());
    
    m_storage->deleteUser([userName](const User &x) -> bool {
        return x.getName() == userName;
    });
    m_storage->deleteMeeting([](const Meeting &x) -> bool {
        return x.getParticipator().empty();
    });
}

std::list<User> AgendaService::listAllUsers(void) const {
    return m_storage->queryUser([](const User & x) -> bool {
        return true;
    });
}

void AgendaService::createMeeting(const std::string &userName, const std::string &title, const std::string &startDate, const std::string &endDate, const std::vector<std::string> &participator) {
    // user basic check
    checkUserExistence(userName);                                   // the sponsor exists?
    if (participator.empty()) {
        throw ParticipatorEmptyException(title);                    // no participators
    }

    std::set<std::string> partpool;
    for (auto n: participator) {
        if (partpool.find(n) != partpool.end()) {
            throw ParticipatorDuplicateException(title, n);         // duplicate participator
        }
        if (n == userName) {
            throw SponsorIsParticipatorExcpetion(title, n);         // sponsor is participator
        }
        checkUserExistence(n);                                      // the participator exists?
        partpool.insert(n);
    }
    
    // time basic check
    checkTimeFormat(startDate);                                     // dates are invalid?
    checkTimeFormat(endDate);                                       // dates are invalid?
    Date sd(startDate), ed(endDate);
    if (sd >= ed) {
        throw TimeIntervalInvalidException(startDate, endDate);     // the start date is not earlier than end date
    }
    
    // meeting info basic check
    if (!m_storage->queryMeeting([title](const Meeting & x) -> bool {
        return x.getTitle() == title;
    }).empty()) {
        throw MeetingDuplicateException(title);                     // duplicate title
    }
    
    // free time check
    checkUserBusy(userName, sd, ed);                                // sponsor is busy
    for (auto n: participator) {
        checkUserBusy(n, sd, ed);                                   // participator is busy
    }
    
    // all done
    m_storage->createMeeting(Meeting(userName, participator, sd, ed, title));
}

void AgendaService::addMeetingParticipator(const std::string &userName, const std::string &title, const std::string &participator) {
    checkUserExistence(userName);                      // the sponsor does not exist
    checkUserExistence(participator);
    
    auto m = getMeetingBySponsorAndTitle(userName, title);          // should check the existence of meeting and permission
    if (m.isParticipator(participator)) {
        throw ParticipatorDuplicateException(title, participator);  // already in
    }
    
    if (userName == participator) {
        throw IsSponsorException(title, userName);                  // sponsor is participator
    }
    
    if (m.isParticipator(userName)) {
        throw ParticipatorDuplicateException(title, userName);      // username is participator
    }
    
    checkUserBusy(participator, m.getStartDate(), m.getEndDate());  // participator is busy
    
    // all done, maybe
    int updated = m_storage->updateMeeting([title, userName](const Meeting &x) -> bool {
        return x.getSponsor() == userName && x.getTitle() == title;
    }, [participator](Meeting &x) -> void {
        x.addParticipator(participator);
    });
    if (updated <= 0) {
        throw MeetingNotExistException(title);
    }
}

void AgendaService::removeMeetingParticipator(const std::string &userName, const std::string &title, const std::string &participator) {
    checkUserExistence(userName);
    checkUserExistence(participator);
    
    auto m = getMeetingBySponsorAndTitle(userName, title);          // should check the existence of meeting and permission
    
    if (userName == participator) {
        throw IsSponsorException(title, userName);      // sponsor cannot remove itself
    }

    if (!m.isParticipator(participator)) {
        throw NotAMemberOfMeetingException(title, participator);    // not in the meeting
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
        throw UnknownException();
    }
}

void AgendaService::quitMeeting(const std::string &userName, const std::string &title) {
    checkUserExistence(userName);

    auto ls = m_storage->queryMeeting([title](const Meeting &x) -> bool {
        return x.getTitle() == title;
    });
    if (ls.size() != 1) {
        throw MeetingNotExistException(title);          // given title exists?
    }
    auto m = *ls.begin();
    if (m.getSponsor() == userName) {
        throw IsSponsorException(title, userName);      // I delete sponsor?
    }
    if (!m.isParticipator(userName)) {
        throw NotAMemberOfMeetingException(title, userName);    // Am I in it?
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
        throw UnknownException();
    }
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName, const std::string &title) const {
    checkUserExistence(userName);
    
    return m_storage->queryMeeting([userName, title](const Meeting &x) -> bool {
        return x.getTitle() == title && (x.getSponsor() == userName || x.isParticipator(userName));
    });
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName, const std::string &startDate, const std::string &endDate) const {
    checkUserExistence(userName);
    checkTimeFormat(startDate);
    checkTimeFormat(endDate);
    
    Date sd(startDate), ed(endDate);
    if (sd > ed) {
        throw TimeIntervalInvalidException(startDate, endDate);     // query date invalid
    }
    
    return m_storage->queryMeeting([userName, sd, ed](const Meeting &x) -> bool {
        return (x.getSponsor() == userName || x.isParticipator(userName)) && x.getEndDate() >= sd && x.getStartDate() <= ed;
    });
}

std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {
    checkUserExistence(userName);
    
    return m_storage->queryMeeting([userName](const Meeting &x) -> bool {
        return x.getSponsor() == userName || x.isParticipator(userName);
    });
}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {
    checkUserExistence(userName);

    return m_storage->queryMeeting([userName](const Meeting &x) -> bool {
        return x.getSponsor() == userName;
    });
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(const std::string &userName) const {
    checkUserExistence(userName);
    
    return m_storage->queryMeeting([userName](const Meeting &x) -> bool {
        return x.isParticipator(userName);
    });
}

void AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {
    checkUserExistence(userName);
    getMeetingBySponsorAndTitle(userName, title);
    
    int deleted = m_storage->deleteMeeting([userName, title](const Meeting &x) -> bool {
        return (x.getSponsor() == userName) && (x.getTitle() == title);
    });
    if (deleted <= 0) {
        throw UnknownException();
    }
}

void AgendaService::deleteAllMeetings(const std::string &userName) {
    checkUserExistence(userName);
    
    m_storage->deleteMeeting([userName](const Meeting &x) -> bool {
        return x.getSponsor() == userName;
    });
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
