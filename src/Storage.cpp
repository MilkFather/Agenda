#include "Storage.hpp"
#include "Path.hpp"
#include "Date.hpp"
#include "CSV.hpp"
#include "AgendaLog.hpp"
#include <fstream>
#include <vector>

std::shared_ptr<Storage> Storage::m_instance(NULL);

Storage::Storage(void) {
    this->readFromFile();
}

void Storage::log(const std::string s) const {
    AgendaLogMan::getInstance()->Log("AgendaStorage: " + s);
}

bool Storage::readFromFile(void) {
    this->m_userList.clear();
    this->m_meetingList.clear();
    std::vector<std::vector<std::string> > users, meetings;
    users = CSV::openFile(Path::userPath);
    meetings = CSV::openFile(Path::meetingPath);
    for (auto ul : users) {
        if (ul.size() != 4)
            continue;
        this->m_userList.push_back(User(ul[0], ul[1], ul[2], ul[3]));
    }
    for (auto ml : meetings) {
        if (ml.size() != 5)
            continue;
        std::vector<std::string> parts = CSV::split(ml[1], '&');
        this->m_meetingList.push_back(Meeting(ml[0], parts, Date::stringToDate(ml[2]), Date::stringToDate(ml[3]),ml[4]));
    }

    log("read users: " + std::to_string(m_userList.size()) + " entries.");
    log("read meetings: " + std::to_string(m_meetingList.size()) + " entries.");

    this->m_dirty = false;
    return true;
}

bool Storage::writeToFile(void) {
    vector<CSVRow> users, meetings;
    for (auto ul : this->m_userList) {
        CSVRow r = {ul.getName(), 
                    ul.getPassword(), 
                    ul.getEmail(), 
                    ul.getPhone()};
        users.push_back(r);
    }
    for (auto ml : this->m_meetingList) {
        CSVRow r = {ml.getSponsor(),
                    CSV::unlines(ml.getParticipator(), "&"),
                    Date::dateToString(ml.getStartDate()),
                    Date::dateToString(ml.getEndDate()),
                    ml.getTitle()};
        meetings.push_back(r);
    }
    CSV::writeFile(users, Path::userPath);
    CSV::writeFile(meetings, Path::meetingPath);

    log("wrote users: " + std::to_string(m_userList.size()) + " entries.");
    log("wrote meetings: " + std::to_string(m_meetingList.size()) + " entries.");

    this->m_dirty = false;
    return true;
}

std::shared_ptr<Storage> Storage::getInstance(void) {  // static
    if (Storage::m_instance.get() == NULL) {
        Storage::m_instance = std::shared_ptr<Storage>(new Storage());
    }
    return Storage::m_instance;
}

Storage::~Storage() {
    if (this->m_dirty) {
        this->writeToFile();
    }
}

void Storage::createUser(const User & t_user) {
    this->m_userList.push_back(t_user);
    this->m_dirty = true;

    log("created 1 user entries.");
}

std::list<User>
Storage::queryUser(std::function<bool(const User &)> filter) const {
    std::list<User> res;
    std::list<User>::const_iterator it = this->m_userList.cbegin();
    for (; it != this->m_userList.cend(); it++) {
        if (filter(*it) == true) {
            res.push_back(*it);
        }
    }

    log("queried " + std::to_string(res.size()) + " user entries.");

    return res;
}

int Storage::updateUser(std::function<bool(const User &)> filter,
                        std::function<void(User &)> switcher) {
    int count = 0;
    std::list<User>::iterator it = this->m_userList.begin();
    for (; it != this->m_userList.end(); it++) {
        if (filter(*it) == true) {
            switcher(*it);
            count++;
        }
    }

    log("updated " + std::to_string(count) + " user entries.");

    this->m_dirty = true;
    return count;
}

int Storage::deleteUser(std::function<bool(const User &)> filter) {
    int count = 0;
    for (auto it = this->m_userList.begin(); it != this->m_userList.end(); ) {
        if (filter(*it) == true) {
            it = this->m_userList.erase(it);
            count++;
        } else {
            ++it;
        }
    }

    log("deleted " + std::to_string(count) + " user entries.");

    this->m_dirty = true;
    return count;
}

void Storage::createMeeting(const Meeting & t_meeting) {
    this->m_meetingList.push_back(t_meeting);
    this->m_dirty = true;

    log("created 1 meeting entries.");
}

std::list<Meeting>
Storage::queryMeeting(std::function<bool(const Meeting &)> filter) const {
    std::list<Meeting> res;
    std::list<Meeting>::const_iterator it = this->m_meetingList.cbegin();
    for (; it != this->m_meetingList.cend(); it++) {
        if (filter(*it) == true) {
            res.push_back(*it);
        }
    }

    log("queried " + std::to_string(res.size()) + " meeting entries.");

    return res;
}

int Storage::updateMeeting(std::function<bool(const Meeting &)> filter,
                           std::function<void(Meeting &)> switcher) {
    int count = 0;
    std::list<Meeting>::iterator it = this->m_meetingList.begin();
    for (; it != this->m_meetingList.end(); it++) {
        if (filter(*it) == true) {
            switcher(*it);
            count++;
        }
    }

    log("updated " + std::to_string(count) + " meeting entries.");

    this->m_dirty = true;
    return count;
}

int Storage::deleteMeeting(std::function<bool(const Meeting &)> filter) {
    int count = 0;
    std::list<Meeting> ls = this->queryMeeting(filter);
    auto it = this->m_meetingList.begin();
    for (; it != this->m_meetingList.end(); ) {
        if (filter(*it) == true) {
            it = this->m_meetingList.erase(it);
            count++;
        } else {
            ++it;
        }
    }

    log("deleted " + std::to_string(count) + " meeting entries.");

    this->m_dirty = true;
    return count;
}

bool Storage::sync(void) {
    this->m_dirty = false;
    return this->writeToFile();
}
