#include "AgendaUI.hpp"
#include "AgendaException.hpp"
#include <exception>
#include <algorithm>

using std::exception;

const string greenbold = "\033[1;32m";
const string redbold = "\033[1;31m";
const string resetcon = "\033[0m";

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
    std::cout << std::endl << "Agenda 1.0" << std::endl << std::endl;
    std::cout << "Copyright 2018 Sun Yat-sen University, all rights reserved." << std::endl;
    std::cout << "For a list of available commands, please type \"h\"" << std::endl;
    std::cout << std::endl;
}

AgendaUI::~AgendaUI() {
    this->m_agendaService.quitAgenda();
}

void AgendaUI::OperationLoop(void) {
    bool cont;
    do {
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
        if (t_operation == "h")
            this->printActions();
        else if (t_operation == "o")
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
        if (t_operation == "h")
            this->printActions();
        else if (t_operation == "l")
            this->userLogIn();
        else if (t_operation == "r")
            this->userRegister();
        else if (t_operation == "q") {
            this->quitAgenda();
            return false;
        }
    }
    return true;
}

void AgendaUI::printActions(void) {
    std::cout
        << "----------------------- Agenda -------------------------------"
        << std::endl;
    std::cout << "Action :" << std::endl;
    if (this->m_userName == "") {  // not logged in
        std::cout << "h   - show this list" << std::endl;
        std::cout
            << "l   - log in Agenda by user name and password"
            << std::endl;
        std::cout << "r   - register an Agenda account" << std::endl;
        std::cout << "q   - quit Agenda" << std::endl;
    } else {  // logged in
        std::cout << "h   - show this list" << std::endl;
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
}

void AgendaUI::userLogIn(void) {
    try {
        std::cout << "[log in] [user name] [password]" << std::endl;
        std::cout << "[log in] ";

        std::string u, p;
        std::cin >> u >> p;

        this->m_agendaService.userLogIn(u, p);      // should throw on fail

        this->m_userName = u;
        this->m_userPassword = p;
        std::cout << greenbold << "[log in] success" << resetcon << std::endl;
    } catch (exception &e) {
        std::cout << redbold << "[log in] " << e.what() << resetcon << std::endl;
        std::cout << redbold << "[log in] fail" << resetcon << std::endl;
    }
    
    std::cout << std::endl;
}

void AgendaUI::userRegister(void) {
    try {
        std::cout
            << "[register] [user name] [password] [email] [phone]"
            << std::endl;

        std::cout << "[register] ";
        std::string u, p, e, t;
        std::cin >> u >> p >> e >> t;
        this->m_agendaService.userRegister(u, p, e, t);     // should throw on fail
        std::cout << greenbold << "[register] success" << resetcon << std::endl;
    } catch (exception &e) {
        std::cout << redbold << "[register] " << e.what() << resetcon << std::endl;
        std::cout << redbold << "[register] fail" << resetcon << std::endl;
    }
    
    std::cout << std::endl;
}

void AgendaUI::userLogOut(void) {
    try {
        this->m_userName = "";
        this->m_userPassword = "";
    } catch (exception &e) {
        std::cout << redbold << "[user logout] " << e.what() << resetcon << std::endl;
        std::cout << redbold << "[user logout] fail" << resetcon << std::endl;
    }

    std::cout << std::endl;
}

void AgendaUI::quitAgenda(void) {
    try {
        this->m_agendaService.quitAgenda();
    } catch (exception &e) {
        std::cout << redbold << "[quit agenda] " << e.what() << resetcon << std::endl;
        std::cout << redbold << "[quit agenda] fail" << resetcon << std::endl;
    }

    std::cout << std::endl;
}

void AgendaUI::deleteUser(void) {
    try {
        this->m_agendaService.deleteUser(this->m_userName, this->m_userPassword);
        this->m_userName = "";
        this->m_userPassword = "";
        std::cout << greenbold << "[delete agenda account] success" << resetcon << std::endl;
    } catch (exception &e) {
        std::cout << redbold << "[delete agenda account] " << e.what() << resetcon << std::endl;
        std::cout << redbold << "[delete agenda account] fail" << resetcon << std::endl;
    }

    std::cout << std::endl;
}

void AgendaUI::listAllUsers(void) {
    try {
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
    } catch (exception &e) {
        std::cout << redbold << "[list all users] " << e.what() << resetcon << std::endl;
        std::cout << redbold << "[list all users] fail" << resetcon << std::endl;
    }
    
    std::cout << std::endl;
}

void AgendaUI::createMeeting(void) {
    try {
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

        this->m_agendaService.createMeeting (this->m_userName, t, st, et, parts);       // should throw on fail
        std::cout << greenbold << "[create meeting] success" << resetcon << std::endl;
    } catch (exception &e) {
        std::cout << redbold << "[create meeting] " << e.what() << resetcon << std::endl;
        std::cout << redbold << "[create meeting] fail" << resetcon << std::endl;
    }
    
    std::cout << std::endl;
}

void AgendaUI::addParticipator(void) {
    try {
        std::cout << "[add participator] [meeting title] [participator username]" << std::endl;  // NOLINT
        std::cout << "[add participator] ";
        std::string t, p;
        std::cin >> t >> p;
        this->m_agendaService.addMeetingParticipator(this->m_userName, t, p);       // should throw on fail
        std::cout << greenbold << "[add participator] success" << resetcon << std::endl;
    } catch (exception &e) {
        std::cout << redbold << "[add participator] " << e.what() << resetcon << std::endl;
        std::cout << redbold << "[add participator] fail" << resetcon << std::endl;
    }
    
    std::cout << std::endl;
}

void AgendaUI::removeParticipator(void) {
    try {
        std::cout << "[remove participator] [meeting title] [participator username]" << std::endl;  // NOLINT
        std::cout << "[remove participator] ";
        std::string t, p;
        std::cin >> t >> p;
        this->m_agendaService.removeMeetingParticipator(this->m_userName, t, p);     // should throw on fail
        std::cout << greenbold << "[remove participator] success" << resetcon << std::endl;
    } catch (exception &e) {
        std::cout << redbold << "[remove participator] " << e.what() << resetcon << std::endl;
        std::cout << redbold << "[remove participator] fail" << resetcon << std::endl;
    }
    
    std::cout << std::endl;
}

void AgendaUI::quitMeeting(void) {
    try {
        std::cout << "[quit meeting] [meeting title]" << std::endl;
        std::cout << "[quit meeting] ";
        std::string t;
        std::cin >> t;
        this->m_agendaService.quitMeeting(this->m_userName, t);                     // should throw on fail
        std::cout << greenbold << "[quit meeting] success" << resetcon << std::endl;
    } catch (exception &e) {
        std::cout << redbold << "[quit meeting] " << e.what() << resetcon << std::endl;
        std::cout << redbold << "[quit meeting] fail" << resetcon << std::endl;
    }
    
    std::cout << std::endl;
}

void AgendaUI::listAllMeetings(void) {
    try {
        std::cout << "[list all meetings]" << std::endl;
        std::cout << std::endl;
        this->printMeetings(this->m_agendaService.listAllMeetings(this->m_userName));
    } catch (exception &e) {
        std::cout << redbold << "[list all meetings] " << e.what() << resetcon << std::endl;
        this->printMeetings(std::list<Meeting>());
    }
    
    std::cout << std::endl;
}

void AgendaUI::listAllSponsorMeetings(void) {
    try {
        std::cout << "[list all sponsor meetings]" << std::endl;
        std::cout << std::endl;
        this->printMeetings(this->m_agendaService.listAllSponsorMeetings(this->m_userName));
    } catch (exception &e) {
        std::cout << redbold << "[list all sponsor meetings] " << e.what() << resetcon << std::endl;
        this->printMeetings(std::list<Meeting>());
    }
    
    std::cout << std::endl;
}

void AgendaUI::listAllParticipateMeetings(void) {
    try {
        std::cout << "[list all participator meetings]" << std::endl;
        std::cout << std::endl;
        this->printMeetings(this->m_agendaService.listAllParticipateMeetings(this->m_userName));
    } catch (exception &e) {
        std::cout << redbold << "[list all participator meetings] " << e.what() << resetcon << std::endl;
        this->printMeetings(std::list<Meeting>());
    }

    std::cout << std::endl;
}

void AgendaUI::queryMeetingByTitle(void) {
    try {
        std::cout << "[query meeting] [title]" << std::endl;
        std::cout << "[query meeting] ";
        std::string t;
        std::cin >> t;
        std::cout << std::endl;
        this->printMeetings(this->m_agendaService.meetingQuery(this->m_userName, t));
    } catch (exception &e) {
        std::cout << redbold << "[query meeting] " << e.what() << resetcon << std::endl;
        this->printMeetings(std::list<Meeting>());
    }

    std::cout << std::endl;
}

void AgendaUI::queryMeetingByTimeInterval(void) {
    try {
        std::cout
            << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]"  // NOLINT
            << std::endl;

        std::cout << "[query meetings] ";
        std::string st, et;
        std::cin >> st >> et;
        std::cout << std::endl;
        this->printMeetings(this->m_agendaService.meetingQuery(this->m_userName, st, et));
    } catch (exception &e) {
        std::cout << redbold << "[query meetings] " << e.what() << resetcon << std::endl;
        this->printMeetings(std::list<Meeting>());
    }
    
    std::cout << std::endl;
}

void AgendaUI::deleteMeetingByTitle(void) {
    try {
        std::cout << "[delete meeting] [title]" << std::endl;
        std::cout << "[delete meeting] ";
        std::string t;
        std::cin >> t;
        this->m_agendaService.deleteMeeting(this->m_userName, t);           // should throw on fail
        std::cout << greenbold << "[delete meeting by title] success" << resetcon << std::endl;
    } catch (exception &e) {
        std::cout << redbold << "[delete meeting by title] " << e.what() << resetcon << std::endl;
        std::cout << redbold << "[delete meeting by title] fail" << resetcon << std::endl;
    }
    
    std::cout << std::endl;
}

void AgendaUI::deleteAllMeetings(void) {
    try {
        this->m_agendaService.deleteAllMeetings(this->m_userName);          // should throw on fail
        std::cout << greenbold << "[delete all meetings] success" << resetcon << std::endl;
    } catch (exception &e) {
        std::cout << redbold << "[delete all meetings] " << e.what() << resetcon << std::endl;
        std::cout << redbold << "[delete all meetings] fail" << resetcon << std::endl;
    }
    
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
