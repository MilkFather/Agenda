#ifndef AGENDASERVICE_H
#define AGENDASERVICE_H

#include <list>
#include <string>
#include "Storage.hpp"

using std::string;
using std::list;
using std::vector;

class AgendaService {
public:
    /**
     * constructor
     */
    AgendaService();

    /**
     * destructor
     */
    ~AgendaService();

    /**
     * check if the username match password
     * @param userName the username want to login
     * @param password the password user enter
     * @return if success, true will be returned
     */
    void userLogIn(const string &userName, const string &password);

    /**
     * regist a user
     * @param userName new user's username
     * @param password new user's password
     * @param email new user's email
     * @param phone new user's phone
     * @return if success, true will be returned
     */
    void userRegister(const string &userName, const string &password,
                      const string &email, const string &phone);

    /**
     * change user information
     * @param userName the user's username, used to identify and cannot be changed
     * @param newemail the user's new email
     * @param newphone the user's new phone
     * @return if success, no error is thrown
     */
    void changeEmailAndPhone(const string &userName, const string &newemail, const string &newphone);

    /**
     * change a user's password
     * @param userName the user's username, used to identify
     * @param oldPassword the user's old password, used to verify identity
     * @param newPassword the user's new password
     */
    void changePassword(const string &userName, const string &oldPassword, const string &newPassword);

    /**
     * delete a user
     * @param userName user's username
     * @param password user's password
     * @return if success, true will be returned
     */
    void deleteUser(const string &userName, const string &password, const bool &action = true);

    /**
     * list all users from storage
     * @return a user list result
     */
    list<User> listAllUsers(void) const;

    /**
     * create a meeting
     * @param userName the sponsor's userName
     * @param title the meeting's title
     * @param participator the meeting's participator
     * @param startData the meeting's start date
     * @param endData the meeting's end date
     * @return if success, true will be returned
     */
    void createMeeting(const string &userName, const string &title,
                       const string &startDate, const string &endDate,
                       const vector<string> &participator);

    /**
     * add a participator to a meeting
     * @param userName the sponsor's userName
     * @param title the meeting's title
     * @param participator the meeting's participator
     * @return if success, true will be returned
     */
    void addMeetingParticipator(const string &userName,
                                const string &title,
                                const string &participator);

    /**
     * remove a participator from a meeting
     * @param userName the sponsor's userName
     * @param title the meeting's title
     * @param participator the meeting's participator
     * @return if success, true will be returned
     */
    void removeMeetingParticipator(const string &userName,
                                   const string &title,
                                   const string &participator,
                                   const bool &action = true);

    /**
     * quit from a meeting
     * @param userName the current userName. need to be the participator (a sponsor can not quit his/her meeting)
     * @param title the meeting's title
     * @return if success, true will be returned
     */
    void quitMeeting(const string &userName, const string &title, const bool &action = true);

    /**
     * search a meeting by username and title
     * @param userName as a sponsor OR a participator
     * @param title the meeting's title
     * @return a meeting list result
     */
    list<Meeting> meetingQuery(const string &userName,
                                    const string &title) const;
    /**
     * search a meeting by username, time interval
     * @param userName as a sponsor OR a participator
     * @param startDate time interval's start date
     * @param endDate time interval's end date
     * @return a meeting list result
     */
    list<Meeting> meetingQuery(const string &userName,
                                    const string &startDate,
                                    const string &endDate) const;

    /**
     * list all meetings the user take part in
     * @param userName user's username
     * @return a meeting list result
     */
    list<Meeting> listAllMeetings(const string &userName) const;

    /**
     * list all meetings the user sponsor
     * @param userName user's username
     * @return a meeting list result
     */
    list<Meeting> listAllSponsorMeetings(const string &userName) const;

    /**
     * list all meetings the user take part in and sponsor by other
     * @param userName user's username
     * @return a meeting list result
     */
    list<Meeting> listAllParticipateMeetings(
        const string &userName) const;

    /**
     * delete a meeting by title and its sponsor
     * @param userName sponsor's username
     * @param title meeting's title
     * @return if success, true will be returned
     */
    void deleteMeeting(const string &userName, const string &title, const bool &action = true);

    /**
     * delete all meetings by sponsor
     * @param userName sponsor's username
     * @return if success, true will be returned
     */
    void deleteAllMeetings(const string &userName, const bool &action = true);

    /**
     * start Agenda service and connect to storage
     */
    void startAgenda(void);

    /**
     * quit Agenda service
     */
    void quitAgenda(void);

private:
    shared_ptr<Storage> m_storage;

    /**
     * helper functions
     */
    void checkUserExistence(const string userName) const; 
    void checkTimeFormat(const string t) const;
    void checkUserBusy(const string usr, const Date sd, const Date ed) const;
    Meeting getMeetingBySponsorAndTitle(const string sponsor, const string title) const;

    void log(const string l) const;
};

#endif
