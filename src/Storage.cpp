#include "Storage.hpp"
#include "Path.hpp"
#include "Date.hpp"
#include <fstream>
#include <vector>

std::shared_ptr<Storage> Storage::m_instance(NULL);

Storage::Storage(void) {
    this->readFromFile();
}

std::vector<std::string> split(std::string s, char c) {
    std::vector<std::string> v;
    std::string tmp;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == c) {
            v.push_back(tmp);
            tmp = "";
        } else {
            tmp += s[i];
        }
    }
    if (tmp != "") {
        v.push_back(tmp);
    }
    return v;
}

std::vector<std::string> CSVparse(std::string s) {
    // rules: entries are separated by ",",
    // each entry is enclosed by a pair of brackets. Other rules may apply.
    // assumption: no entry shall have a comma, a quote, or &.
    std::vector<std::string> v = split(s, ',');
    for (int i = 0; i < v.size(); i++) {
        v[i] = v[i].substr(1, v[i].length() - 2);
    }
    return v;
}

std::string CSVize(std::string s) {
    // strip any newline, any comma, any quote and any &,
    // before they are enclosed with quotes and written to the file.
    std::string proc;
    for (int i = 0; i < s.length(); i++) {
        if (s[i] == '\n' || s[i] == ',' || s[i] == '\"' || s[i] == '&') {
            continue;
        } else {
            proc += s[i];
        }
    }
    return proc;
}

std::string flat(std::vector<std::string> f, char c) {
    std::string s;
    if (f.size() <= 0)
        return s;
    s += f[0];
    for (int i = 1; i < f.size(); i++) {
        s += c;
        s += CSVize(f[i]);
    }
    return s;
}

bool Storage::readFromFile(void) {
    std::ifstream usr(Path::userPath), meeting(Path::meetingPath);
    std::string l;
    if (usr.is_open()) {
        while (getline(usr, l)) {
            std::vector<std::string> usrs = CSVparse(l);
            if (usrs.size() != 4) {
                this->m_userList.clear();
                return false;
            } else {
                User u(usrs[0], usrs[1], usrs[2], usrs[3]);
                this->createUser(u);
            }
        }
    } else {
        this->m_userList.clear();
        return false;
    }

    if (meeting.is_open()) {
        while (getline(meeting, l)) {
            std::vector<std::string> meet = CSVparse(l);
            if (meet.size() != 5) {
                this->m_meetingList.clear();
                return false;
            } else {
                Date st = Date::stringToDate(meet[2]);
                Date et = Date::stringToDate(meet[3]);

                std::vector<std::string> parts = split(meet[1], '&');

                Meeting m(meet[0], parts, st, et, meet[4]);
                this->createMeeting(m);
            }
        }
    } else {
        this->m_meetingList.clear();
        return false;
    }
    this->m_dirty = false;
    return true;
}

bool Storage::writeToFile(void) {
    std::ofstream usr(Path::userPath, std::fstream::out);
    std::ofstream meeting(Path::meetingPath, std::fstream::out);
    std::string l;
    if (usr.is_open()) {
        std::list<User>::iterator it = this->m_userList.begin();
        for (; it != this->m_userList.end(); it++) {
            User u = *it;
            usr << "\"" << CSVize(u.getName()) << "\",\""
            << CSVize(u.getPassword()) << "\",\""
            << CSVize(u.getEmail()) << "\",\""
            << CSVize(u.getPhone()) << "\"";
            auto it2 = it;
            it2++;
            if (it2 != this->m_userList.end()) {
                usr << std::endl;
            }
        }
        usr.close();
    } else {
        return false;
    }

    if (meeting.is_open()) {
        std::list<Meeting>::iterator it = this->m_meetingList.begin();
        for (; it != this->m_meetingList.end(); it++) {
            Meeting m = *it;
            meeting << "\"" << CSVize(m.getSponsor()) << "\",\""
            << flat(m.getParticipator(), '&') << "\",\""
            << CSVize(Date::dateToString(m.getStartDate())) << "\",\""
            << CSVize(Date::dateToString(m.getEndDate())) << "\",\""
            << CSVize(m.getTitle()) << "\"";
            auto it2 = it;
            it2++;
            if (it2 != this->m_meetingList.end()) {
                meeting << std::endl;
            }
        }
        meeting.close();
    } else {
        return false;
    }
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
    this->m_dirty = true;
    return count;
}

void Storage::createMeeting(const Meeting & t_meeting) {
    this->m_meetingList.push_back(t_meeting);
    this->m_dirty = true;
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
    this->m_dirty = true;
    return count;
}

bool Storage::sync(void) {
    this->m_dirty = false;
    return this->writeToFile();
}
