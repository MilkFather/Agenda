#include "Path.hpp"

#ifdef _WIN32
const char *Path::meetingPath = "/tmp/meetings.csv";
const char *Path::userPath = "/tmp/users.csv";
const char *Path::logDirPath = "\\tmp\\AgendaLog\\";
#else
const char *Path::meetingPath = "/tmp/meetings.csv";
const char *Path::userPath = "/tmp/users.csv";
const char *Path::logDirPath = "/tmp/AgendaLog/";
#endif