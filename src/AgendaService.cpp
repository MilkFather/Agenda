#include "AgendaService.hpp"
#include <functional>
#include "Meeting.hpp"
#include "User.hpp"

AgendaService::AgendaService() {
    this->m_storage = Storage::getInstance();
}

AgendaService::~AgendaService() {

}

bool AgendaService::userLogin(const std::string &userName, const std::string &password) {
    std::function<bool(const User &)> filter = [userName, password]bool(const User & x) {return (x.getName() == userName && x.getPassword() == password);};
    return (this->m_storage->get()->queryUser(filter).size() > 0);
}

bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                                 const std::string &email, const std::string &phone) {
    std::function<bool(const User &)> filter = [userName]bool(const User & x) {return (x.getName == userName);};
    if (this->m_storage->get()->queryUser(filter).size() > 0) {
        return false;  // creating existing user name.
    } else {
        this->m_storage->get()->createUser(User(userName, password, email, phone));
        return true;
    }
}

bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {
    std::function<bool(const User &)> filter = [userName, password]bool(const User & x) {return (x.getName() == userName && x.getPassword() == password);};
    if (this->m_storage->get()->queryUser(filter).size() > 0) {
        // we are going to delete this user
        // as well as any meeting schedule associated with it.
        std::function<bool(const Meeting &)> mfilter = [userName]bool(const Meeting & x) {x.getSponsor() == userName || x.isParticipator(userName);};
        this->m_storage->get()->deleteMeeting(mfilter);
        this->m_storage->get()->deleteUser(filter);
        return true;
    } else {
        return false;  // incorrect username or password
    }
}

std::list<User> AgendaService::listAllUsers(void) const {
    std::function<bool(const User &)> all = []bool(const User & x) {return true;};
    return this->m_storage->get()->queryUser(all);
}

bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                                  const std::string &startDate, const std::string &endDate,
                                  const std::vector<std::string> &participator) {
    // we  have multiple checks.
    // 1. does this username really exist? does every participator exist?
    std::function<bool(const User &) sponsorfilter = [userName]bool(const User &x) {return x.getName() == userName;};
    if (this->m_storage->get()->queryUser(sponsorfilter).size() <= 0) {
        return false;  // the sponsor does not exist
    } else {
        for (int i = 0; i < participator.size(); i++) {
            std::function<bool(const User &) partfilter = [participator, i]bool(const User & x) {return x.getName() == participator[i];};
            if (this->m_storage->get()->queryUser(partfilter)->size() <= 0) {
                return false;  // one of the participators does not exist
            }
        }
    }
    // 2. 
}

bool AgendaService::addMeetingParticipator(const std::string &userName,
                                           const std::string &title,
                                           const std::string &participator) {
                                            
}

bool AgendaService::removeMeetingParticipator(const std::string &userName,
                                              const std::string &title,
                                              const std::string &participator)  {

}

bool AgendaService::quitMeeting(const std::string &userName, const std::string &title) {

}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                               const std::string &title) const {

}

std::list<Meeting> AgendaService::meetingQuery(const std::string &userName,
                                               const std::string &startDate,
                                               const std::string &endDate) const {

}

std::list<Meeting> AgendaService::listAllMeetings(const std::string &userName) const {

}

std::list<Meeting> AgendaService::listAllSponsorMeetings(const std::string &userName) const {

}

std::list<Meeting> AgendaService::listAllParticipateMeetings(
    const std::string &userName) const {

}

bool AgendaService::deleteMeeting(const std::string &userName, const std::string &title) {

}

bool AgendaService::deleteAllMeetings(const std::string &userName) {

}

void AgendaService::startAgenda(void) {

}

void AgendaService::quitAgenda(void) {
    
}