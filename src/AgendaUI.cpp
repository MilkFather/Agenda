#include "AgendaUI.hpp"
#include "AgendaException.hpp"
#include <exception>
#include <algorithm>
#include <cstdarg>
#include <cctype>
#include <vector>
#include "AgendaLog.hpp"

using std::exception;

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

bool AgendaUI::printConfirmation(const std::string &text) const {
    char c;
    do {
        printActionHeader(yellowbold);
        std::cout << yellowbold << " " << text << " [Y/N] " << resetcon;
        std::cin >> c;
    } while (std::toupper(c) != 'Y' && std::toupper(c) != 'N');
    return (std::toupper(c) == 'Y');
}

void AgendaUI::printActionHeader(const std::string style) const {
    std::cout << style << "[" << m_currentAction << "]" << resetcon;
}

void AgendaUI::printParameter(const int n, ...) const {
    va_list args;
    va_start(args, n);
    printActionHeader();
    for (int i = 0; i < n; i++) {
        const char* p;
        p = va_arg(args, const char*);
        std::cout << bold << " [" << p << "]" << resetcon;
    }
    va_end(args);
    std::cout << std::endl;
}

void AgendaUI::getParameter(const int n, ...) const {
    va_list args;
    va_start(args, n);
    printActionHeader();
    std::cout << " ";
    for (int i = 0; i < n; i++) {
        char* p;
        p = va_arg(args, char*);
        std::cin >> p;
    }
    va_end(args);
}

void AgendaUI::printError(const std::string &msg) const {
    printActionHeader(redbold);
    std::cout << redbold << " Error: " << msg << resetcon << std::endl;
    printActionHeader(redbold);
    std::cout << redbold << " Failed" << resetcon << std::endl << std::endl;
    AgendaLogMan::getInstance()->Log("Error");
}

void AgendaUI::printSuccess() const {
    printActionHeader(greenbold);
    std::cout << greenbold << " Success" << resetcon << std::endl << std::endl;
}

void AgendaUI::printCancel() const {
    printActionHeader(bluebold);
    std::cout << bluebold << " Cancelled" << resetcon << std::endl << std::endl;
}

AgendaUI::AgendaUI() {
    std::cout << std::endl << "Agenda 1.0" << std::endl << std::endl;
    std::cout << "Copyright 2018 Sun Yat-sen University, all rights reserved." << std::endl;
    std::cout << "For a list of available commands, please type \"" << bold << "h" << resetcon << "\"" << std::endl;
    std::cout << std::endl;
}

AgendaUI::~AgendaUI() {
    this->m_agendaService.quitAgenda();
}

void AgendaUI::OperationLoop(void) {
    bool cont;
    do {
        m_currentAction = "";
        std::string cmd = this->getOperation();
        cont = this->executeOperation(cmd);
    } while (cont);
}

void AgendaUI::startAgenda(void) {
    this->m_agendaService.startAgenda();
}

std::string AgendaUI::getOperation() {
    std::cout
        << (this->m_userName == "" ? "Agenda" : this->m_userName + "@Agenda")
        << " :~"
        << (this->m_userName == "" ? "$ " : "# ");

    std::string op;
    std::cin >> op;
    std::cout << std::endl;
    return op;
}

bool AgendaUI::executeOperation(std::string t_operation) {
    if (this->m_userName != "") {  // logged in
        if (t_operation == "h") {
            m_currentAction = "PRINT COMMANDS";
            this->printActions();
        } else if (t_operation == "o") {
            m_currentAction = "LOG OUT";
            this->userLogOut();
        } else if (t_operation == "dc") {
            m_currentAction = "DELETE AGENDA ACCOUNT";
            this->deleteUser();
        } else if (t_operation == "lu") {
            m_currentAction = "LIST ALL USERS";
            this->listAllUsers();
        } else if (t_operation == "cm") {
            m_currentAction = "CREATE MEETING";
            this->createMeeting();
        } else if (t_operation == "amp") {
            m_currentAction = "ADD PARTICIPATOR";
            this->addParticipator();
        } else if (t_operation == "rmp") {
            m_currentAction = "REMOVE PARTICIPATOR";
            this->removeParticipator();
        } else if (t_operation == "rqm") {
            m_currentAction = "QUIT MEETING";
            this->quitMeeting();
        } else if (t_operation == "la") {
            m_currentAction = "LIST ALL MEETINGS";
            this->listAllMeetings();
        } else if (t_operation == "las") {
            m_currentAction = "LIST ALL SPONSOR MEETINGS";
            this->listAllSponsorMeetings();
        } else if (t_operation == "lap") {
            m_currentAction = "LIST ALL PARTICIPATOR MEETINGS";
            this->listAllParticipateMeetings();
        } else if (t_operation == "qm") {
            m_currentAction = "QUERY MEETING";
            this->queryMeetingByTitle();
        } else if (t_operation == "qt") {
            m_currentAction = "QUERY MEETINGS";
            this->queryMeetingByTimeInterval();
        } else if (t_operation == "dm") {
            m_currentAction = "DELETE MEETING";
            this->deleteMeetingByTitle();
        } else if (t_operation == "da") {
            m_currentAction = "DELETE ALL MEETINGS";
            this->deleteAllMeetings();
        }
    } else {   // not logged in
        if (t_operation == "h") {
            m_currentAction = "PRINT COMMANDS";
            this->printActions();
        } else if (t_operation == "l") {
            m_currentAction = "LOG IN";
            this->userLogIn();
        } else if (t_operation == "r") {
            m_currentAction = "REGISTER";
            this->userRegister();
        } else if (t_operation == "q") {
            m_currentAction = "QUIT AGENDA";
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
        std::cout << bold << "h" << resetcon << "   - Show this list" << std::endl;
        std::cout
            << bold
            << "l"
            << resetcon 
            << "   - Log in Agenda by user name and password"
            << std::endl;
        std::cout << bold << "r" << resetcon << "   - Register an Agenda account" << std::endl;
        std::cout << bold << "q" << resetcon << "   - Quit Agenda" << std::endl;
    } else {  // logged in
        std::cout << bold << "h" << resetcon << "   - Show this list" << std::endl;
        std::cout << bold << "o" << resetcon << "   - Log out Agenda" << std::endl;
        std::cout << bold << "dc" << resetcon << "  - Delete Agenda account" << std::endl;
        std::cout << bold << "lu" << resetcon << "  - List all Agenda user" << std::endl;
        std::cout << bold << "cm" << resetcon << "  - Create a meeting" << std::endl;
        std::cout << bold << "amp" << resetcon << " - Add meeting participator" << std::endl;
        std::cout << bold << "rmp" << resetcon << " - Remove meeting participator" << std::endl;
        std::cout << bold << "rqm" << resetcon << " - Request to quit meeting" << std::endl;
        std::cout << bold << "la" << resetcon << "  - List all meetings" << std::endl;
        std::cout << bold << "las" << resetcon << " - List all sponsor meetings" << std::endl;
        std::cout << bold << "lap" << resetcon << " - List all participate meetings" << std::endl;
        std::cout << bold << "qm" << resetcon << "  - Query meeting by title" << std::endl;
        std::cout << bold << "qt" << resetcon << "  - Query meeting by time interval" << std::endl;
        std::cout << bold << "dm" << resetcon << "  - Delete meeting by title" << std::endl;
        std::cout << bold << "da" << resetcon << "  - Delete all meetings" << std::endl;
    }
    std::cout
        << "--------------------------------------------------------------"
        << std::endl;

    std::cout << std::endl;
}

void AgendaUI::userLogIn(void) {
    try {
        printParameter(2, "user name", "password");

        char u[1024], p[1024];
        getParameter(2, u, p);

        this->m_agendaService.userLogIn(u, p);      // should throw on fail

        this->m_userName = u;
        this->m_userPassword = p;
        printSuccess();
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::userRegister(void) {
    try {
        printParameter(4, "user name", "password", "email", "phone");
        char u[1024], p[1024], e[1024], t[1024];
        getParameter(4, u, p, e, t);
        this->m_agendaService.userRegister(u, p, e, t);     // should throw on fail
        printSuccess();
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::userLogOut(void) {
    try {
        this->m_userName = "";
        this->m_userPassword = "";
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::quitAgenda(void) {
    try {
        this->m_agendaService.quitAgenda();
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::deleteUser(void) {
    try {
        this->m_agendaService.deleteUser(this->m_userName, this->m_userPassword, false);

        if (printConfirmation("Do you want to delete this account?")) {
            this->m_agendaService.deleteUser(this->m_userName, this->m_userPassword);
            this->m_userName = "";
            this->m_userPassword = "";
            printSuccess();
        } else {
            printCancel();
        }
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::listAllUsers(void) {
    try {
        printActionHeader();
        std::cout << std::endl << std::endl;
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
            << bold
            << "name" << fillblank(widname, 4)
            << "email" << fillblank(widemail, 5)
            << "phone" << fillblank(widphone, 5)
            << resetcon
            << std::endl;

        for (int i = 0; i < usrnames.size(); i++) {
            std::cout
                << usrnames[i] << fillblank(widname, usrnames[i].length())
                << emails[i] << fillblank(widemail, emails[i].length())
                << phones[i] << fillblank(widphone, phones[i].length())
                << std::endl;
        }
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::createMeeting(void) {
    try {
        printParameter(1, "the number of participators");
        char numstr[1024];
        int num;
        getParameter(1, &numstr);
        num = std::stoi(numstr);
        std::vector<std::string> parts;
        for (int i = 1; i <= num; i++) {
            printParameter(1, std::string("please enter the participator " + std::to_string(i)).c_str());
            char name[1024];
            getParameter(1, name);
            parts.push_back(name);
        }
        printParameter(3, "title", "start time(yyyy-mm-dd/hh:mm)", "end time(yyyy-mm-dd/hh:mm)");
        char t[1024], st[1024], et[1024];
        getParameter(3, t, st, et);

        this->m_agendaService.createMeeting (this->m_userName, t, st, et, parts);       // should throw on fail
        printSuccess();
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::addParticipator(void) {
    try {
        printParameter(2, "meeting title", "participator username");
        char t[1024], p[1034];
        getParameter(2, t, p);
        this->m_agendaService.addMeetingParticipator(this->m_userName, t, p);       // should throw on fail
        printSuccess();
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::removeParticipator(void) {
    try {
        printParameter(2, "meeting title", "participator username");
        char t[1024], p[1024];
        getParameter(2, t, p);
        this->m_agendaService.removeMeetingParticipator(this->m_userName, t, p, false);     // should throw on fail
        
        if (printConfirmation("Do you want to remove \"" + string(p) + "\" from \"" + string(t) + "\"?")) {
            this->m_agendaService.removeMeetingParticipator(this->m_userName, t, p);
            printSuccess();
        } else {
            printCancel();
        }
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::quitMeeting(void) {
    try {
        printParameter(1, "meeting title");
        char t[1024];
        getParameter(1, t);
        this->m_agendaService.quitMeeting(this->m_userName, t, false);                     // should throw on fail
        
        if (printConfirmation("Do you want to quit from \"" + std::string(t) + "\"?")) {
            this->m_agendaService.quitMeeting(this->m_userName, t);
            printSuccess();
        } else {
            printCancel();
        }
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::listAllMeetings(void) {
    try {
        printActionHeader(bold);
        std::cout << std::endl << std::endl;
        this->printMeetings(this->m_agendaService.listAllMeetings(this->m_userName));
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::listAllSponsorMeetings(void) {
    try {
        printActionHeader(bold);
        std::cout << std::endl << std::endl;
        this->printMeetings(this->m_agendaService.listAllSponsorMeetings(this->m_userName));
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::listAllParticipateMeetings(void) {
    try {
        printActionHeader(bold);
        std::cout << std::endl << std::endl;
        this->printMeetings(this->m_agendaService.listAllParticipateMeetings(this->m_userName));
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::queryMeetingByTitle(void) {
    try {
        printParameter(1, "title");
        char t[1024];
        getParameter(1, t);
        std::cout << std::endl;
        this->printMeetings(this->m_agendaService.meetingQuery(this->m_userName, t));
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::queryMeetingByTimeInterval(void) {
    try {
        printParameter(2, "start time(yyyy-mm-dd/hh:mm)", "end time(yyyy-mm-dd/hh:mm)");
        char st[1024], et[1024];
        getParameter(2, st, et);
        std::cout << std::endl;
        this->printMeetings(this->m_agendaService.meetingQuery(this->m_userName, st, et));
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::deleteMeetingByTitle(void) {
    try {
        printParameter(1, "title");
        char t[1024];
        getParameter(1, t);
        this->m_agendaService.deleteMeeting(this->m_userName, t, false);           // should throw on fail

        if (printConfirmation("Do you want to delete the meeting \"" + string(t) + "\"?")) {
            this->m_agendaService.deleteMeeting(this->m_userName, t);  
            printSuccess();
        } else {
            printCancel();
        }
    } catch (exception &e) {
        printError(e.what());
    }
}

void AgendaUI::deleteAllMeetings(void) {
    try {
        this->m_agendaService.deleteAllMeetings(this->m_userName, false);          // should throw on fail

        if (printConfirmation("Do you want to delete all meetings?")) {
            this->m_agendaService.deleteAllMeetings(this->m_userName); 
            printSuccess();
        } else {
            printCancel();
        }
    } catch (exception &e) {
        printError(e.what());
    }
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

void AgendaUI::printMeetings(const std::list<Meeting> t_meetings) const {
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
        << bold
        << "title" << fillblank(widtitle, 5)
        << "sponsor" << fillblank(widsponsor, 7)
        << "start time" << fillblank(widsts, 10)
        << "end time" << fillblank(widets, 8)
        << "participators" << fillblank(widparts, 13)
        << resetcon
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
