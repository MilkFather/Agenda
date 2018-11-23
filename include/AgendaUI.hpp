#ifndef AGENDAUI_H
#define AGENDAUI_H

#include "AgendaService.hpp"
#include <iostream>
#include <string>
#include <vector>

#define resetcon "\033[0m"
#define bold "\033[1m"
#define redbold "\033[1;31m"
#define greenbold "\033[1;32m"
#define yellowbold "\033[1;33m"
#define bluebold "\033[1;34m"

class AgendaUI {
public:
    AgendaUI();
    ~AgendaUI();
    void OperationLoop(void);

private:
    /**
     * constructor
     */
    void startAgenda(void);

    /**
     * catch user's operation
     * @return the operation
     */
    std::string getOperation();

    /**
     * execute the operation
     * @return if the operationloop continue
     */
    bool executeOperation(std::string t_operation);

    /**
     * print available commands
     */
    void printActions(void);

    /**
     * user Login
     */
    void userLogIn(void);

    /**
     * user regist
     */
    void userRegister(void);

    /**
     * user information change
     */
    void userChangeEmailPhone(void);

    /**
     * user password change
     */
    void userChangePassword(void);

    /**
     * user logout
     */
    void userLogOut(void);

    /**
     * quit the Agenda
     */
    void quitAgenda(void);

    /**
     * delete a user from storage
     */
    void deleteUser(void);

    /**
     * list all users from storage
     */
    void listAllUsers(void);

    /**
     * user create a meeting with someone else
     */
    void createMeeting(void);

    /**
     * user add a participator
     */
    void addParticipator(void);

    /**
     * user remove a participator
     */
    void removeParticipator(void);

    /**
     * user quit from a meeting
     */
    void quitMeeting(void);

    /**
     * list all meetings from storage
     */
    void listAllMeetings(void);

    /**
     * list all meetings that this user sponsored
     */
    void listAllSponsorMeetings(void);

    /**
     * list all meetings that this user take part in
     */
    void listAllParticipateMeetings(void);

    /**
     * search meetings by title from storage
     */
    void queryMeetingByTitle(void);

    /**
     * search meetings by timeinterval from storage
     */
    void queryMeetingByTimeInterval(void);

    /**
     * delete meetings by title from storage
     */
    void deleteMeetingByTitle(void);

    /**
     * delete all meetings that this user sponsored
     */
    void deleteAllMeetings(void);

    /**
     * show the meetings in the screen
     */
    void printMeetings(const std::list<Meeting> t_meetings) const;

    /**
     * user confirmation
     */
    bool printConfirmation(const std::string &text) const;

    /**
     * show the header
     */
    void printActionHeader(const std::string style = bold) const;

    /**
     * show action parameters
     */
    void printParameter(const int n, ...) const;

    /**
     * get input parameters
     */
    void getParameter(const int n, ...) const;

    /**
     * show error
     */
    void printError(const std::string &msg) const;

    /**
     * show success
     */
    void printSuccess() const;

    /**
     * show cancel
     */
    void printCancel() const;

    void log(const std::string s) const;

    // data
    std::string m_userName;
    std::string m_userPassword;
    AgendaService m_agendaService;
    std::string m_currentAction;
};

#endif
