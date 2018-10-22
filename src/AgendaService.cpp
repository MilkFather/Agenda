#include "AgendaService.hpp"

AgendaService::AgendaService() {

}

AgendaService::~AgendaService() {

}

bool AgendaService::userLogin(const std::string &userName, const std::string &password) {

}

bool AgendaService::userRegister(const std::string &userName, const std::string &password,
                                 const std::string &email, const std::string &phone) {
    
}

bool AgendaService::deleteUser(const std::string &userName, const std::string &password) {

}

std::list<User> AgendaService::listAllUsers(void) const {

}

bool AgendaService::createMeeting(const std::string &userName, const std::string &title,
                                  const std::string &startDate, const std::string &endDate,
                                  const std::vector<std::string> &participator) {

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