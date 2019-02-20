#include "AgendaService.hpp"
#include <set>
#include "AgendaException.hpp"
#include "AgendaLog.hpp"

using std::set;

AgendaService::AgendaService() {
    startAgenda();
}

AgendaService::~AgendaService() {
    quitAgenda();
}

void AgendaService::log(const string l) const {
    AgendaLogMan::getInstance()->Log("AgendaService: " + l);
}

void AgendaService::checkUserExistence(const string userName) const {
    log("starting user existence check...");

    if (m_storage->queryUser([userName](const User &x) -> bool {
        return (x.getName() == userName);
    }).size() != 1) {
        log("user existence check failed.");

        throw UserNotExistException(userName);          // throw when not exist
    }
    log("user existence check passed.");
}

void AgendaService::checkTimeFormat(const string t) const {
    log("starting time validation check...");

    if (!Date::isValid(Date(t))) {
        log("time validation check failed.");

        throw TimeInvalidException(t);                  // dates are invalid
    }

    log("time validation check passed.");
}

void AgendaService::checkUserBusy(const string usr, const Date sd, const Date ed) const {
    log("starting user busy check...");

    if (!m_storage->queryMeeting([usr, sd, ed](const Meeting &x) -> bool {
        return ((x.getSponsor() == usr || x.isParticipator(usr))) && x.getStartDate() < ed && x.getEndDate() > sd;
    }).empty()) {
        log("user busy check failed");

        throw UserBusyException(usr, Date::dateToString(sd), Date::dateToString(ed));                   // is busy
    }

    log("user busy check passed.");
}

Meeting AgendaService::getMeetingBySponsorAndTitle(const string sponsor, const string title) const {
    log("starting find meeting test...");

    auto ls = m_storage->queryMeeting([title](const Meeting &x) -> bool {
        return x.getTitle() == title;
    });
    if (ls.size() != 1) {
        log("find meeting test failed.");

        throw MeetingNotExistException(title);
    }

    log("find meeting test passed.");
    log("starting access permission check...");

    auto m = *ls.begin();
    if (m.getSponsor() != sponsor) {
        log("access permission check failed.");

        throw MeetingAccessDeniedExcpetion(title, sponsor);
    }
    log("access permission check passed.");

    return m;
}

void AgendaService::userLogIn(const string &userName, const string &password) {
    log("starting login check...");

    if (m_storage->queryUser([userName, password](const User & x) -> bool {
        return (x.getName() == userName && x.getPassword() == password);
    }).empty()) {
        log("login check failed.");

        throw UserPasswordNotMatchException();
    }

    log("login check passed.");
}

void AgendaService::userRegister(const string &userName, const string &password, const string &email, const string &phone) {
    log("starting user duplication check...");

    if (!m_storage->queryUser([userName](const User &x) -> bool {
        return (x.getName() == userName);
    }).empty()) {
        log("user duplication check failed.");

        throw UsernameDuplicateException(userName);     // creating existing user name.
    }

    log("user duplication check passed.");

    m_storage->createUser(User(userName, password, email, phone));
}

void AgendaService::changeEmailAndPhone(const string &userName, const string &newemail, const string &newphone) {
    checkUserExistence(userName);

    int updated = m_storage->updateUser([userName](const User &x) -> bool {
        return (x.getName() == userName);
    }, [newemail, newphone](User &x) -> void {
        x.setEmail(newemail);
        x.setPhone(newphone);
    });

    if (updated != 1) {
        throw UnknownException();
    }
}

void AgendaService::changePassword(const string &userName, const string &oldPassword, const string &newPassword) {
    userLogIn(userName, oldPassword);

    int updated = m_storage->updateUser([userName](const User &x) -> bool {
        return (x.getName() == userName);
    }, [newPassword](User &x) -> void {
        x.setPassword(newPassword);
    });

    if (updated != 1) {
        throw UnknownException();
    }
}

void AgendaService::deleteUser(const string &userName, const string &password, const bool &action) {
    userLogIn(userName, password);                      // the correct user & password
    
    if (action) {   // allow mock delete
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
}

list<User> AgendaService::listAllUsers(void) const {
    return m_storage->queryUser([](const User & x) -> bool {
        return true;
    });
}

void AgendaService::createMeeting(const string &userName, const string &title, const string &startDate, const string &endDate, const vector<string> &participator) {
    // user basic check
    checkUserExistence(userName);                                   // the sponsor exists?

    log("starting participator empty check...");

    if (participator.empty()) {
        log("participator empty check failed.");

        throw ParticipatorEmptyException(title);                    // no participators
    }

    log("participator empty check passed.");
    log("starting participator check...");

    set<string> partpool;
    for (auto n: participator) {
        if (partpool.find(n) != partpool.end()) {
            log("participator check failed: duplication.");

            throw ParticipatorDuplicateException(title, n);         // duplicate participator
        }
        if (n == userName) {
            log("participator check failed: sponsor is participator.");

            throw SponsorIsParticipatorExcpetion(title, n);         // sponsor is participator
        }
        checkUserExistence(n);                                      // the participator exists?
        partpool.insert(n);
    }

    log("participator check passed.");
    
    // time basic check
    checkTimeFormat(startDate);                                     // dates are invalid?
    checkTimeFormat(endDate);                                       // dates are invalid?

    log("starting time interval check...");

    Date sd(startDate), ed(endDate);
    if (sd >= ed) {
        log("time interval check failed.");

        throw TimeIntervalInvalidException(startDate, endDate);     // the start date is not earlier than end date
    }

    log("time interval check passed.");
    
    // meeting info basic check
    log("starting meeting duplication check...");

    if (!m_storage->queryMeeting([title](const Meeting & x) -> bool {
        return x.getTitle() == title;
    }).empty()) {
        log("meeting duplication check failed.");

        throw MeetingDuplicateException(title);                     // duplicate title
    }

    log("meeting duplication check passed.");
    
    // free time check
    checkUserBusy(userName, sd, ed);                                // sponsor is busy
    for (auto n: participator) {
        checkUserBusy(n, sd, ed);                                   // participator is busy
    }
    
    // all done
    m_storage->createMeeting(Meeting(userName, participator, sd, ed, title));
}

void AgendaService::addMeetingParticipator(const string &userName, const string &title, const string &participator) {
    checkUserExistence(userName);                      // the sponsor does not exist
    checkUserExistence(participator);
    
    auto m = getMeetingBySponsorAndTitle(userName, title);          // should check the existence of meeting and permission

    log("starting participator duplication check...");

    if (m.isParticipator(participator)) {
        log("participator duplication check failed.");

        throw ParticipatorDuplicateException(title, participator);  // already in
    }

    log("participator duplication check passed.");
    log("starting sponsor is participator check...");
    
    if (userName == participator) {
        log("sponsor is participator check failed.");

        throw IsSponsorException(title, userName);                  // sponsor is participator
    }

    log("sponsor is participator check passed.");
    log("starting sponsor duplication check...");
    
    if (m.isParticipator(userName)) {
        log("sponsor duplication check failed.");

        throw ParticipatorDuplicateException(title, userName);      // username is participator
    }

    log("sponsor duplication check passed.");
    
    checkUserBusy(participator, m.getStartDate(), m.getEndDate());  // participator is busy
    
    // all done, maybe
    int updated = m_storage->updateMeeting([title, userName](const Meeting &x) -> bool {
        return x.getSponsor() == userName && x.getTitle() == title;
    }, [participator](Meeting &x) -> void {
        x.addParticipator(participator);
    });

    log("starting final check...");

    if (updated <= 0) {
        log("final check failed.");

        throw MeetingNotExistException(title);
    }

    log("final check passed.");
}

void AgendaService::removeMeetingParticipator(const string &userName, const string &title, const string &participator, const bool &action) {
    checkUserExistence(userName);
    checkUserExistence(participator);
    
    auto m = getMeetingBySponsorAndTitle(userName, title);          // should check the existence of meeting and permission
    
    log("starting sponsor is participator check...");

    if (userName == participator) {
        log("sponsor is participator check failed.");

        throw IsSponsorException(title, userName);      // sponsor cannot remove itself
    }

    log("sponsor is participator check passed.");
    log("starting participator check...");

    if (!m.isParticipator(participator)) {
        log("participator check failed.");

        throw NotAMemberOfMeetingException(title, participator);    // not in the meeting
    }

    log("participator check passed");
    
    if (action) {   // allow mock remove
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
}

void AgendaService::quitMeeting(const string &userName, const string &title, const bool &action) {
    checkUserExistence(userName);

    log("starting meeting existence check...");

    auto ls = m_storage->queryMeeting([title](const Meeting &x) -> bool {
        return x.getTitle() == title;
    });
    if (ls.size() != 1) {
        log("meeting existence check failed.");

        throw MeetingNotExistException(title);          // given title exists?
    }

    log("meeting existence passed.");
    log("starting is-sponsor check...");

    auto m = *ls.begin();
    if (m.getSponsor() == userName) {
        log("is-sponsor check failed.");

        throw IsSponsorException(title, userName);      // I delete sponsor?
    }

    log("is-sponsor check passed.");
    log("starting is-participator check...");

    if (!m.isParticipator(userName)) {
        log("is-participator check failed.");

        throw NotAMemberOfMeetingException(title, userName);    // Am I in it?
    }

    log("is-participator check passed.");
    
    if (action) {   // allow mock quit
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
}

list<Meeting> AgendaService::meetingQuery(const string &userName, const string &title) const {
    checkUserExistence(userName);
    
    return m_storage->queryMeeting([userName, title](const Meeting &x) -> bool {
        return x.getTitle() == title && (x.getSponsor() == userName || x.isParticipator(userName));
    });
}

list<Meeting> AgendaService::meetingQuery(const string &userName, const string &startDate, const string &endDate) const {
    checkUserExistence(userName);
    checkTimeFormat(startDate);
    checkTimeFormat(endDate);
    
    log("starting time interval validation check...");

    Date sd(startDate), ed(endDate);
    if (sd > ed) {
        log("time interval validation check failed.");

        throw TimeIntervalInvalidException(startDate, endDate);     // query date invalid
    }

    log("time interval validation check passed.");
    
    return m_storage->queryMeeting([userName, sd, ed](const Meeting &x) -> bool {
        return (x.getSponsor() == userName || x.isParticipator(userName)) && x.getEndDate() >= sd && x.getStartDate() <= ed;
    });
}

list<Meeting> AgendaService::listAllMeetings(const string &userName) const {
    checkUserExistence(userName);
    
    return m_storage->queryMeeting([userName](const Meeting &x) -> bool {
        return x.getSponsor() == userName || x.isParticipator(userName);
    });
}

list<Meeting> AgendaService::listAllSponsorMeetings(const string &userName) const {
    checkUserExistence(userName);

    return m_storage->queryMeeting([userName](const Meeting &x) -> bool {
        return x.getSponsor() == userName;
    });
}

list<Meeting> AgendaService::listAllParticipateMeetings(const string &userName) const {
    checkUserExistence(userName);
    
    return m_storage->queryMeeting([userName](const Meeting &x) -> bool {
        return x.isParticipator(userName);
    });
}

void AgendaService::deleteMeeting(const string &userName, const string &title, const bool &action) {
    checkUserExistence(userName);
    getMeetingBySponsorAndTitle(userName, title);
    
    if (action) {   // allow mock delete
        int deleted = m_storage->deleteMeeting([userName, title](const Meeting &x) -> bool {
            return (x.getSponsor() == userName) && (x.getTitle() == title);
        });
        if (deleted <= 0) {
            throw UnknownException();
        }
    }
}

void AgendaService::deleteAllMeetings(const string &userName, const bool &action) {
    checkUserExistence(userName);
    
    if (action) {
        m_storage->deleteMeeting([userName](const Meeting &x) -> bool {
            return x.getSponsor() == userName;
        });
    }
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
