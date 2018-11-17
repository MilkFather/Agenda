#include "AgendaUI.hpp"

int maxlength(std::vector<std::string> v) {
    int l = 0;
    for (int i = 0; i < v.size(); i++)
        l = std::max(l, static_cast<int>(v[i].length()));
    return l;
}

std::string fillblank(int total, int used) {
    std::string s;
    for (int i = 0; i < (total - used); i++)
        s += " ";
    return s;
}

AgendaUI::AgendaUI() {
}

void AgendaUI::OperationLoop(void) {
    bool cont;
    do {
        std::cout
        << "----------------------- Agenda -------------------------------"
        << std::endl;
        std::cout << "Action :" << std::endl;
        if (this->m_userName == "") {  // not logged in
            std::cout
                << "l   - log in Agenda by user name and password"
                << std::endl;
            std::cout << "r   - register an Agenda account" << std::endl;
            std::cout << "q   - quit Agenda" << std::endl;
        } else {  // logged in
            std::cout << "o   - log out Agenda" << std::endl;
            std::cout << "dc  - delete Agenda account" << std::endl;
            std::cout << "lu  - list all Agenda user" << std::endl;
            std::cout << "cm  - create a meeting" << std::endl;
            std::cout << "amp - add meeting participator" << std::endl;
            std::cout << "rmp - remove meeting participator" << std::endl;
            std::cout << "rqm - request to quit meeting" << std::endl;
            std::cout << "la  - list all meetings" << std::endl;
            std::cout << "las - list all sponsor meetings" << std::endl;
            std::cout << "lap - list all participate meetings" << std::endl;
            std::cout << "qm  - query meeting by title" << std::endl;
            std::cout << "qt  - query meeting by time interval" << std::endl;
            std::cout << "dm  - delete meeting by title" << std::endl;
            std::cout << "da  - delete all meetings" << std::endl;
        }
        std::cout
            << "--------------------------------------------------------------"
            << std::endl;

        std::cout << std::endl;

        std::string cmd = this->getOperation();
        cont = this->executeOperation(cmd);
    } while (cont);
}

void AgendaUI::startAgenda(void) {
    this->m_agendaService.startAgenda();
}

std::string AgendaUI::getOperation() {
    std::cout
        << (this->m_userName == "" ? "Agenda" : "Agenda@" + this->m_userName)
        << " :~"
        << (this->m_userName == "" ? "$ " : "# ");

    std::string op;
    std::cin >> op;
    std::cout << std::endl;
    return op;
}

bool AgendaUI::executeOperation(std::string t_operation) {
    if (this->m_userName != "") {  // logged in
        if (t_operation == "o")
            this->userLogOut();
        else if (t_operation == "dc")
            this->deleteUser();
        else if (t_operation == "lu")
            this->listAllUsers();
        else if (t_operation == "cm")
            this->createMeeting();
        else if (t_operation == "amp")
            this->addParticipator();
        else if (t_operation == "rmp")
            this->removeParticipator();
        else if (t_operation == "rqm")
            this->quitMeeting();
        else if (t_operation == "la")
            this->listAllMeetings();
        else if (t_operation == "las")
            this->listAllSponsorMeetings();
        else if (t_operation == "lap")
            this->listAllParticipateMeetings();
        else if (t_operation == "qm")
            this->queryMeetingByTitle();
        else if (t_operation == "qt")
            this->queryMeetingByTimeInterval();
        else if (t_operation == "dm")
            this->deleteMeetingByTitle();
        else if (t_operation == "da")
            this->deleteAllMeetings();
    } else {   // not logged in
        if (t_operation == "l") {
            this->userLogIn();
        } else if (t_operation == "r") {
            this->userRegister();
        } else if (t_operation == "q") {
            this->quitAgenda();
            return false;
        }
    }
    return true;
}

void AgendaUI::userLogIn(void) {
    std::cout << "[log in] [user name] [password]" << std::endl;
    std::cout << "[log in] ";

    std::string u, p;
    std::cin >> u >> p;
    if (this->m_agendaService.userLogIn(u, p) == true) {
        this->m_userName = u;
        this->m_userPassword = p;
        std::cout << "[log in] succeed!" << std::endl;
    } else {
        std::cout << "[log in] Password error or user doesn\'t exist" << std::endl;
    }
    std::cout << std::endl;
}

void AgendaUI::userRegister(void) {
    std::cout
        << "[register] [user name] [password] [email] [phone]"
        << std::endl;

    std::cout << "[register] ";

    std::string u, p, e, t;
    std::cin >> u >> p >> e >> t;
    if (this->m_agendaService.userRegister(u, p, e, t)) {
        std::cout << "[register] succeed!" << std::endl;
    } else {
        std::cout << "[register] This username has been registered!" << std::endl;
    }
    std::cout << std::endl;
}

void AgendaUI::userLogOut(void) {
    this->m_userName = "";
    this->m_userPassword = "";
    std::cout << std::endl;
}

void AgendaUI::quitAgenda(void) {
    this->m_agendaService.quitAgenda();
    std::cout << std::endl;
}

void AgendaUI::deleteUser(void) {
    this->m_agendaService.deleteUser(this->m_userName, this->m_userPassword);
    this->m_userName = "";
    this->m_userPassword = "";
    std::cout << "[delete agenda account] succeed!" << std::endl;
    std::cout << std::endl;
}

void AgendaUI::listAllUsers(void) {
    std::cout << "[list all users]" << std::endl;
    std::cout << std::endl;
    std::list<User> ul = this->m_agendaService.listAllUsers();
    // prewrite to vectors
    std::vector<std::string> usrnames, emails, phones;
    auto it = ul.begin();
    for (; it != ul.end(); ++it) {
        usrnames.push_back((*it).getName());
        emails.push_back((*it).getEmail());
        phones.push_back((*it).getPhone());
    }
    // width?
    int widname, widemail, widphone;
    widname = std::max(4, maxlength(usrnames)) + 3;
    widemail = std::max(5, maxlength(emails)) + 3;
    widphone = std::max(5, maxlength(phones)) + 3;
    // write!
    std::cout
        << "name" << fillblank(widname, 4)
        << "email" << fillblank(widemail, 5)
        << "phone" << fillblank(widphone, 5)
        << std::endl;

    for (int i = 0; i < usrnames.size(); i++) {
        std::cout
            << usrnames[i] << fillblank(widname, usrnames[i].length())
            << emails[i] << fillblank(widemail, emails[i].length())
            << phones[i] << fillblank(widphone, phones[i].length())
            << std::endl;
    }
    std::cout << std::endl;
}

void AgendaUI::createMeeting(void) {
    std::cout << "[create meeting] [the number of participators]" << std::endl;
    std::cout << "[create meeting] ";
    int num;
    std::vector<std::string> parts;
    std::cin >> num;
    for (int i = 1; i <= num; i++) {
        std::cout
            << "[create meeting] [please enter the participator " << i << " ]"
            << std::endl;

        std::cout << "[create meeting] ";
        std::string name;
        std::cin >> name;
        parts.push_back(name);
    }
    std::cout
        << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"  // NOLINT
        << std::endl;
    std::string t, st, et;
    std::cout << "[create meeting] ";
    std::cin >> t >> st >> et;

    if (this->m_agendaService.createMeeting \
            (this->m_userName, t, st, et, parts) == true) {
        std::cout << "[create meeting] succeed!" << std::endl;
    } else {
        std::cout << "[create meeting] error!" << std::endl;
    }
    std::cout << std::endl;
}

void AgendaUI::addParticipator(void) {
    std::cout << "[add participator] [meeting title] [participator username]" << std::endl;  // NOLINT
    std::cout << "[add participator] ";
    std::string t, p;
    std::cin >> t >> p;
    if (this->m_agendaService.addMeetingParticipator(this->m_userName, t, p)) {
        std::cout << "[add participator] succeed!" << std::endl;
    } else {
        std::cout << "[add participator] error!" << std::endl;
    }
    std::cout << std::endl;
}

void AgendaUI::removeParticipator(void) {
    std::cout << "[remove participator] [meeting title] [participator username]" << std::endl;  // NOLINT
    std::cout << "[remove participator] ";
    std::string t, p;
    std::cin >> t >> p;
    if (this->m_agendaService.removeMeetingParticipator(this->m_userName, t, p)) {
        std::cout << "[remove participator] succeed!" << std::endl;
    } else {
        std::cout << "[remove participator] error!" << std::endl;
    }
    std::cout << std::endl;
}

void AgendaUI::quitMeeting(void) {
    std::cout << "[quit meeting] [meeting title]" << std::endl;
    std::cout << "[quit meeting] ";
    std::string t;
    std::cin >> t;
    if (this->m_agendaService.quitMeeting(this->m_userName, t)) {
        std::cout << "[quit meeting] succeed!" << std::endl;
    } else {
        std::cout << "[quit meeting] error!" << std::endl;
    }
    std::cout << std::endl;
}

void AgendaUI::listAllMeetings(void) {
    std::cout << "[list all meetings]" << std::endl;
    std::cout << std::endl;
    this->printMeetings \
        (this->m_agendaService.listAllMeetings(this->m_userName));
    std::cout << std::endl;
}

void AgendaUI::listAllSponsorMeetings(void) {
    std::cout << "[list all sponsor meetings]" << std::endl;
    std::cout << std::endl;
    this->printMeetings \
        (this->m_agendaService.listAllSponsorMeetings(this->m_userName));
    std::cout << std::endl;
}

void AgendaUI::listAllParticipateMeetings(void) {
    std::cout << "[list all participator meetings]" << std::endl;
    std::cout << std::endl;
    this->printMeetings \
        (this->m_agendaService.listAllParticipateMeetings(this->m_userName));
    std::cout << std::endl;
}

void AgendaUI::queryMeetingByTitle(void) {
    std::cout << "[query meeting] [title]" << std::endl;
    std::cout << "[query meeting] ";
    std::string t;
    std::cin >> t;
    this->printMeetings \
        (this->m_agendaService.meetingQuery(this->m_userName, t));
    std::cout << std::endl;
}

void AgendaUI::queryMeetingByTimeInterval(void) {
    std::cout
        << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"  // NOLINT
        << std::endl;

    std::cout << "[query meetings] ";
    std::string st, et;
    std::cin >> st >> et;
    this->printMeetings \
        (this->m_agendaService.meetingQuery(this->m_userName, st, et));
    std::cout << std::endl;
}

void AgendaUI::deleteMeetingByTitle(void) {
    std::cout << "[delete meeting] [title]" << std::endl;
    std::cout << "[delete meeting] ";
    std::string t;
    std::cin >> t;
    if (this->m_agendaService.deleteMeeting(this->m_userName, t) == true) {
        std::cout << "[delete meeting by title] succeed!" << std::endl;
    } else {
        std::cout << "[error] delete meeting fail!" << std::endl;
    }
    std::cout << std::endl;
}

void AgendaUI::deleteAllMeetings(void) {
    this->m_agendaService.deleteAllMeetings(this->m_userName);
    std::cout << "[delete all meetings] succeed!" << std::endl;
    std::cout << std::endl;
}

std::string unlines(std::vector<std::string> s, char c) {
    if (s.size() == 0)
        return "";
    std::string s1;
    s1 += s[0];
    for (int i = 1; i < s.size(); i++) {
        s1 += c;
        s1 += s[i];
    }
    return s1;
}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
    // prewrite to vectors
    std::vector<std::string> titles, sponsors, sts, ets, parts;
    auto it = t_meetings.begin();
    for (; it != t_meetings.end(); ++it) {
        titles.push_back((*it).getTitle());
        sponsors.push_back((*it).getSponsor());
        sts.push_back(Date::dateToString((*it).getStartDate()));
        ets.push_back(Date::dateToString((*it).getEndDate()));
        parts.push_back(unlines((*it).getParticipator(), ','));
    }
    // width?
    int widtitle, widsponsor, widsts, widets, widparts;
    widtitle = std::max(5, maxlength(titles)) + 3;
    widsponsor = std::max(7, maxlength(sponsors)) + 3;
    widsts = std::max(10, maxlength(sts)) + 3;
    widets = std::max(8, maxlength(ets)) + 3;
    widparts = std::max(13, maxlength(parts)) + 3;
    // write!
    std::cout
        << "title" << fillblank(widtitle, 5)
        << "sponsor" << fillblank(widsponsor, 7)
        << "start time" << fillblank(widsts, 10)
        << "end time" << fillblank(widets, 8)
        << "participators" << fillblank(widparts, 13)
        << std::endl;
    for (int i = 0; i < titles.size(); i++) {
        std::cout
            << titles[i] << fillblank(widtitle, titles[i].length())
            << sponsors[i] << fillblank(widsponsor, sponsors[i].length())
            << sts[i] << fillblank(widsts, sts[i].length())
            << ets[i] << fillblank(widets, ets[i].length())
            << parts[i] << fillblank(widparts, parts[i].length())
            << std::endl;
    }
    std::cout << std::endl;
}
