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
}

void AgendaUI::quitAgenda(void) {

}

void AgendaUI::deleteUser(void) {

}

void AgendaUI::listAllUsers(void) {

}

void AgendaUI::createMeeting(void) {

}

void AgendaUI::listAllMeetings(void) {

}

void AgendaUI::listAllSponsorMeetings(void) {

}

void AgendaUI::listAllParticipateMeetings(void) {

}

void AgendaUI::queryMeetingByTitle(void) {

}

void AgendaUI::queryMeetingByTimeInterval(void) {

}

void AgendaUI::deleteMeetingByTitle(void) {

}

void AgendaUI::deleteAllMeetings(void) {

}

void AgendaUI::printMeetings(std::list<Meeting> t_meetings) {
    
}
