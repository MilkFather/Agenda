#include "AgendaUI.hpp"

AgendaUI::AgendaUI() {

}

void AgendaUI::OperationLoop(void) {
    do {
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

        std::string cmd = this->getOperation();
        bool cont = this->executeOperation(cmd);
    } while (cont);
}

void AgendaUI::startAgenda(void) {
    //this->m_agendaService
}

std::string AgendaUI::getOperation() {

}

bool AgendaUI::executeOperation(std::string t_operation) {

}

void AgendaUI::userLogIn(void) {
    
}

void AgendaUI::userRegister(void) {

}

void AgendaUI::userLogOut(void) {

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