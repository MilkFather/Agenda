#include "AgendaService.hpp"
#include <functional>
#include "Meeting.hpp"
#include "User.hpp"

AgendaService::AgendaService() {
    this->startAgenda();
}

AgendaService::~AgendaService() {
    this->quitAgenda();
}

bool AgendaService::userLogIn(const std::string &userName,
                              const std::string &password) {
    std::function<bool(const User &)> filter =
        [userName, password](const User & x) {
            return (x.getName() == userName && x.getPassword() == password);
        };
    return (this->m_storage.get()->queryUser(filter).size() > 0);
}

bool AgendaService::userRegister(const std::string &userName,
                                 const std::string &password,
                                 const std::string &email,
                                 const std::string &phone) {
    std::function<bool(const User &)> filter =
        [userName](const User & x) {
            return (x.getName() == userName);
        };
    if (this->m_storage.get()->queryUser(filter).size() > 0) {
        return false;  // creating existing user name.
    } else {
        this->m_storage.get()->createUser \
        (User(userName, password, email, phone));
        return true;
    }
}

bool AgendaService::deleteUser(const std::string &userName,
                               const std::string &password) {
    std::function<bool(const User &)> filter =
        [userName, password](const User & x) {
            return (x.getName() == userName && x.getPassword() == password);
        };
    if (this->m_storage.get()->queryUser(filter).size() > 0) {
        // we are going to delete this user
        // as well as any meeting schedule associated with it.
        std::function<bool(const Meeting &)> mfilter =
            [userName](const Meeting & x) {
                return x.getSponsor() == userName || x.isParticipator(userName);
            };

        this->m_storage.get()->deleteMeeting(mfilter);
        this->m_storage.get()->deleteUser(filter);
        return true;
    } else {
        return false;  // incorrect username or password
    }
}

std::list<User> AgendaService::listAllUsers(void) const {
    std::function<bool(const User &)> all =
        [](const User & x) {
            return true;
        };
    return this->m_storage.get()->queryUser(all);
}

bool AgendaService::createMeeting(const std::string &userName,
                                  const std::string &title,
                                  const std::string &startDate,
                                  const std::string &endDate,
                                  const std::vector<std::string>&participator) {
    // we have multiple checks.
    // 1. does this username really exist? does every participator exist?
    std::function<bool(const User &)> sponsorfilter =
        [userName](const User &x) {
            return x.getName() == userName;
        };
    if (this->m_storage.get()->queryUser(sponsorfilter).size() <= 0) {
        return false;  // the sponsor does not exist
    } else {
        for (int i = 0; i < participator.size(); i++) {
            std::function<bool(const User &)> partfilter =
                [participator, i](const User & x) {
                    return x.getName() == participator[i];
                };
            if (this->m_storage.get()->queryUser(partfilter).size() <= 0) {
                return false;  // one of the participators does not exist
            }
        }
    }
    // 2. are the dates valid?
    Date sd(startDate), ed(endDate);
    if (sd == Date(0, 0, 0, 0, 0) || ed == Date(0, 0, 0, 0, 0) || sd >= ed) {
        return false;  // dates are invalid
    }
    // 3. unique title?
    std::function<bool(const Meeting &)> duplicatemeeting =
        [title](const Meeting & x) {
            return x.getTitle() == title;
        };
    if (this->m_storage.get()->queryMeeting(duplicatemeeting).size() > 0) {
        return false;  // duplicate title
    }
    // 4. does everybody have time?
    std::function<bool(const Meeting &)> conflictmeeting =
        [userName, ed, sd, participator](const Meeting &x) {
            if (x.getSponsor() == userName &&
                !(x.getStartDate() >= ed || x.getEndDate() <= sd)) {
                return true;
            }
            for (int i = 0; i < participator.size(); i++) {
                if (x.isParticipator(participator[i]) &&
                    !(x.getStartDate() >= ed || x.getEndDate() <= sd)) {
                    return true;
                }
            }
            return false;
        };
    if (this->m_storage.get()->queryMeeting(conflictmeeting).size() > 0) {
        return false;  // conflict schedule
    }
    // all done
    this->m_storage.get()->createMeeting \
        (Meeting(userName, participator, sd, ed, title));
    return true;
}

bool AgendaService::addMeetingParticipator(const std::string &userName,
                                           const std::string &title,
                                           const std::string &participator) {
    // the meeting exists?
    std::function<bool(const Meeting &)> filter =
        [userName, title](const Meeting &x) {
            return (x.getSponsor() == userName && x.getTitle() == title);
        };
    std::list<Meeting> ls = this->m_storage.get()->queryMeeting(filter);
    if (ls.size() != 1) {
        return false;  // specific meeting not exist or duplicate meetings
    } else if (userName == participator) {
        return false;  // no duplicate people
    } else if ((*(ls.begin())).isParticipator(participator)) {
        return false;  // already in
    } else {
        std::function<bool(const Meeting &)> conflictmeeting =
            [ls, participator](const Meeting &x) {
                if (x.isParticipator(participator) &&
                    !(x.getStartDate() >= (*(ls.begin())).getEndDate() ||
                      x.getEndDate() <= (*(ls.begin())).getStartDate())) {
                    return true;
                }
                return false;
            };
        if (this->m_storage.get()->queryMeeting(conflictmeeting).size() > 0) {
            return false;  // conflict time
        } else {
            std::function<void(Meeting &)> switcher =
                [participator](Meeting &x) {
                    x.addParticipator(participator);
                };
            this->m_storage.get()->updateMeeting(filter, switcher);
            return true;
        }
    }
    return true;
}

bool
    AgendaService::removeMeetingParticipator(const std::string &userName,
                                             const std::string &title,
                                             const std::string &participator)  {
    // the meeting exists?
    std::function<bool(const Meeting &)> filter =
        [userName, title](const Meeting &x) {
            return (x.getSponsor() == userName && x.getTitle() == title);
        };
    std::list<Meeting> ls = this->m_storage.get()->queryMeeting(filter);
    if (ls.size() != 1) {
        return false;  // specific meeting not exist or duplicate meetings
    } else if (userName == participator) {
        return false;  // I'm not participator
    } else if (!(*(ls.begin())).isParticipator(participator)) {
        return false;  // not in
    } else {
        std::function<void(Meeting &)> switcher =
            [participator](Meeting &x) {
                x.removeParticipator(participator);
            };
        this->m_storage.get()->updateMeeting(filter, switcher);
        return true;
    }
    return true;
}

bool AgendaService::quitMeeting(const std::string &userName,
                                const std::string &title) {
    std::function<bool(const Meeting &)> filter =
        [userName, title](const Meeting &x) {
            return (x.getSponsor() != userName &&
                    x.isParticipator(userName) &&
                    x.getTitle() == title);
        };
    if (this->m_storage.get()->queryMeeting(filter).size() != 1) {
        return false;  // not exist or duplicate
    } else {
        std::function<void(Meeting &)> switcher =
            [userName](Meeting &x) {
                x.removeParticipator(userName);
            };
        this->m_storage.get()->updateMeeting(filter, switcher);
        return true;
    }
    return true;
}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                               const std::string &title) const {
    std::function<bool(const Meeting &)> filter =
        [userName, title](const Meeting &x) {
            if (x.getSponsor() != userName && !x.isParticipator(userName)) {
                return false;  // not a member
            }
            if (x.getTitle() != title) {
                return false;
            }
            return true;
        };
    return this->m_storage.get()->queryMeeting(filter);
    std::list<Meeting> t;
    return t;
}

std::list<Meeting>
    AgendaService::meetingQuery(const std::string &userName,
                                const std::string &startDate,
                                const std::string &endDate) const {
    std::function<bool(const Meeting &)> filter =
        [userName, startDate, endDate](const Meeting &x) {
            if (x.getSponsor() != userName && !x.isParticipator(userName)) {
                return false;  // not a member
            }
            if (Date(startDate) == Date(0, 0, 0, 0, 0) ||
                Date(endDate) == Date(0, 0, 0, 0, 0)) {
                return false;  // query date invalid
            }
            if (x.getEndDate() < Date(startDate) ||
                x.getStartDate() > Date(endDate)) {
                return false;  // not in query range
            }
            return true;
        };
    return this->m_storage.get()->queryMeeting(filter);
    std::list<Meeting> t;
    return t;
}

std::list<Meeting>
    AgendaService::listAllMeetings(const std::string &userName) const {
    std::function<bool(const Meeting &)> filter =
        [userName](const Meeting &x) {
            if (x.getSponsor() != userName && !x.isParticipator(userName)) {
                return false;  // not a member
            } else {
                return true;
            }
        };
    return this->m_storage.get()->queryMeeting(filter);
    std::list<Meeting> t;
    return t;
}

std::list<Meeting>
    AgendaService::listAllSponsorMeetings(const std::string &userName) const {
    std::function<bool(const Meeting &)> filter =
        [userName](const Meeting &x) {
            if (x.getSponsor() == userName) {
                return true;
            } else {
                return false;
            }
        };
    return this->m_storage.get()->queryMeeting(filter);
    std::list<Meeting> t;
    return t;
}

std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string &userName) const {
    std::function<bool(const Meeting &)> filter =
        [userName](const Meeting &x) {
            if (x.isParticipator(userName)) {
                return true;
            } else {
                return false;
            }
        };
    return this->m_storage.get()->queryMeeting(filter);

    std::list<Meeting> t;
    return t;
}

bool
    AgendaService::deleteMeeting(const std::string &userName,
                                 const std::string &title) {
    std::function<bool(const Meeting &)> filter =
        [userName, title](const Meeting &x) {
            if (x.getSponsor() != userName) {
                return false;  // not a member
            }
            if (x.getTitle() != title) {
                return false;
            }
            return true;
        };
    return this->m_storage.get()->deleteMeeting(filter);
    return true;
}

bool AgendaService::deleteAllMeetings(const std::string &userName) {
    std::function<bool(const Meeting &)> filter =
        [userName](const Meeting &x) {
            if (x.getSponsor() != userName) {
                return false;  // not a member
            }
            return true;
        };
    return this->m_storage.get()->deleteMeeting(filter);
    return true;
}

void AgendaService::startAgenda(void) {
    this->m_storage = Storage::getInstance();
}

void AgendaService::quitAgenda(void) {
    if (this->m_storage != NULL) {
        this->m_storage.get()->sync();
        this->m_storage = NULL;
    }
}
