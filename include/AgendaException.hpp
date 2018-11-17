#ifndef AGENDAEXCEPTION_H
#define AGENDAEXCEPTION_H

#include <exception>
#include <string>

using std::exception;
using std::string;
using std::stringstream;

class AgendaException: public std::exception {
protected:
    string msg;
public:
    AgendaException(string message): msg(message) {};
    const char* what() const throw() {
        return msg.c_str();
    }
};

class UserPasswordNotMatchException: public AgendaException {
public:
    UserPasswordNotMatchException():
        AgendaException("Incorrect user name or password") {};
};

class UsernameDuplicateException: public AgendaException {
public:
    UsernameDuplicateException(string dup):
        AgendaException("\"" + dup + "\" is already used") {};
};

class UserNotExistException: public AgendaException {
public:
    UserNotExistException(string ne):
        AgendaException("\"" + ne + "\" does not exist") {};
};

class ParticipatorEmptyException: public AgendaException {
public:
    ParticipatorEmptyException(string t):
        AgendaException("\"" + t + "\" has no participators provided") {};
};

class ParticipatorDuplicateException: public AgendaException {
public:
    ParticipatorDuplicateException(string t, string u):
        AgendaException("\"" + u + "\" is duplicate for the meeting \"" + t + "\"") {};
};

class SponsorIsParticipatorExcpetion: public AgendaException {
public:
    SponsorIsParticipatorExcpetion(string t, string usr):
        AgendaException("\"" + usr + "\" is the sponsor of the meeting \"" + t + "\"") {};
};

class TimeInvalidException: public AgendaException {
public:
    TimeInvalidException(string inv):
        AgendaException("\"" + inv + "\" is not a valid string representation of time") {};
};

class TimeIntervalInvalidException: public AgendaException {
public:
    TimeIntervalInvalidException(string s, string d):
        AgendaException("The interval \"" + s + "\" - \"" + d + "\" is invalid") {};
};

class MeetingDuplicateException: public AgendaException {
public:
    MeetingDuplicateException(string t):
        AgendaException("\"" + t + "\" already exists") {};
};

class UserBusyException: public AgendaException {
public:
    UserBusyException(string t, string usr):
        AgendaException("The user \"" + usr + "\" is busy during the meeting \"" + t + "\"") {};
};

class MeetingNotExistException: public AgendaException {
public:
    MeetingNotExistException(string t):
        AgendaException("The requested meeting \"" + t + "\" is not found, or you don\'t have the privilege to access it") {};
};

class NotAMemberOfMeetingException: public AgendaException {
public:
    NotAMemberOfMeetingException(string t, string u):
        AgendaException("\"" + u + "\" is not a member of the meeting \"" + t + "\"") {};
};

#endif
