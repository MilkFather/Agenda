#include "AgendaUI.hpp"

AgendaUI::AgendaUI() {

}

void AgendaUI::OperationLoop(void) {
    bool cont;
    do {
        //std::cout << std::endl;
        std::cout << "----------------------- Agenda -------------------------------" << std::endl;
        std::cout << "Action :" << std::endl;
        if (this->m_userName == "") {  // not logged in    
            std::cout << "l   - log in Agenda by user name and password" << std::endl;
            std::cout << "r   - register an Agenda account" << std::endl;
            std::cout << "q   - quit Agenda" << std::endl; 
        } else {  // logged in
            std::cout << "o   - log out Agenda" << std::endl;
            std::cout << "dc  - delete Agenda account" << std::endl;
            std::cout << "lu  - list all Agenda user" << std::endl;
            std::cout << "cm  - create a meeting" << std::endl;
            std::cout << "la  - list all meetings" << std::endl;
            std::cout << "las - list all sponsor meetings" << std::endl;
            std::cout << "lap - list all participate meetings" << std::endl;
            std::cout << "qm  - query meeting by title" << std::endl;
            std::cout << "qt  - query meeting by time interval" << std::endl;
            std::cout << "dm  - delete meeting by title" << std::endl;
            std::cout << "da  - delete all meetings" << std::endl;
        }
        std::cout << "--------------------------------------------------------------" << std::endl;
        std::cout << std::endl;

        std::string cmd = this->getOperation();
        cont = this->executeOperation(cmd);
    } while (cont);
}

void AgendaUI::startAgenda(void) {
    //this->m_agendaService
}

std::string AgendaUI::getOperation() {
    std::cout
    << (this->m_userName == "" ? "Agenda" : "Agenda@" + this->m_userName) 
    << " : " 
    << (this->m_userName == "" ? "~$ " : "# ");
    
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
        this->m_password = p;
        std::cout << "[log in] succeed!" << std::endl;
    } else {
        std::cout << "[error] log in fail!" << std::endl;
    }
    std::cout << std::endl;
}

void AgendaUI::userRegister(void) {
    std::cout << "[register] [user name] [password] [email] [phone]" << std::endl;
    std::cout << "[register] ";

    std::string u, p, e, t;
    std:cin >> u >> p >> e >> t;
    if (this->m_agendaService.userRegister(u, p, e, t)) {
        std::cout << "[register] succeed!" << std::endl;
    } else {
        std::cout << "[error] register fail!" << std::endl;
    }
    std::cout << std::endl;
}

void AgendaUI::userLogOut(void) {
    this->m_userName = "";
    this->m_password = "";
    std::cout << std::endl;
}

void AgendaUI::quitAgenda(void) {
    this->m_agendaService.quitAgenda();
    std::cout << std::endl;
}

void AgendaUI::deleteUser(void) {
    this->m_agendaService.deleteUser(this->m_userName, this->m_password);
    this->m_userName = "";
    this->m_password = "";
    std::cout << "[delete agenda account] succeed!" << std::endl;
    std::cout << std::endl;
}

void AgendaUI::listAllUsers(void) {
    std::cout << "[list all users]" << std::endl;
    std::cout << std::endl;
    // TODO
}

void AgendaUI::createMeeting(void) {
    std::cout << "[create meeting] [the number of participators]" << std::endl;
    std::cout << "[create meeting] ";
    int num;
    std::vector<std::string> parts;
    std::cin >> num;
    for (int i = 1; i <= num; i++) {
        std::cout << "[create meeting] [please enter the participator " << i << " ]" << std::endl;
        std::cout << "[create meeting] ";
        std::string name;
        std::cin >> name;
        parts.push_back(name);
    }
    std::cout << "[create meeting] [title] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << std::endl;
    std::string t, st, et;
    std::cout << "[create meeting] ";
    cin >> t >> st >> et;

    if (this->m_agendaService.createMeeting(this->m_userName, t, st, et, parts) == true) {
        std::cout << "[create meeting] succeed!" << std::endl;
    } else {
        std::cout << "[create meeting] error!" << std::endl;
    }
    std::cout << std::endl;
}

void AgendaUI::listAllMeetings(void) {
    std::cout << "[list all meetings]" << std::endl;
    std::cout << std::endl;
    this->printMeetings(this->m_agendaService.listAllMeetings(this->m_userName));
    std::cout << std::endl;
}

void AgendaUI::listAllSponsorMeetings(void) {
    std::cout << "[list all sponsor meetings]" << std::endl;
    std::cout << std::endl;
    this->printMeetings(this->m_agendaService.listAllSponsorMeetings(this->m_userName));
    std::cout << std::endl;
}

void AgendaUI::listAllParticipateMeetings(void) {
    std::cout << "[list all participator meetings]" << std::endl;
    std::cout << std::endl;
    this->printMeetings(this->m_agendaService.listAllParticipateMeetings(this->m_userName));
    std::cout << std::endl;
}

void AgendaUI::queryMeetingByTitle(void) {
    std::cout << "[query meeting] [title]" << std::endl;
    std::cout << "[query meeting] " << std::endl;
    std::string t;
    std::cin >> t;
    this->printMeetings(this->m_agendaService.meetingQuery(this->m_userName, t));
    std::cout << std::endl;
}

void AgendaUI::queryMeetingByTimeInterval(void) {
    std::cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << std::endl;
    std::cout << "[query meetings] ";
    std::string st, et;
    std::cin >> st >> et;
    this->printMeetings(this->m_agendaService.meetingQuery(this->m_userName, st, et));
    std::cout << std::endl;
}

void AgendaUI::deleteMeetingByTitle(void) {
    std::cout << "[delete meeting] [title]" << std::endl;
    std::cout << "[delete meeting] ";
    string t;
    std::cin >> t;
    if (this->m_agendaService.deleteMeeting(this->m_userName, t) == true) {
        std::cout << "[delete meeting by title] succeed!" << std::endl;
    } else {
        std::cout << "[error] delete meeting fail!" << std::endl;
    }
    std::cout << std::endl;
}

void AgendaUI::deleteAllMeetings(void) {
    this->m_agendaService.deleteAllMeetings();
    std::cout << "[delete all meetings] succeed!" << std::endl;
    std::cout << std::endl;
}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
    // TODO
}
