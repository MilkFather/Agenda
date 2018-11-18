#ifndef PATH_HPP
#define PATH_HPP

class Path {
 public:
  /**
   * user.csv path
   */
  static const char *userPath;

  /**
   * meeting.csv path
   */
  static const char *meetingPath;

  static const char *logDirPath;
};

const char *Path::meetingPath = "/tmp/meetings.csv";
const char *Path::userPath = "/tmp/users.csv";
const char *Path::logDirPath = "/tmp/AgendaLog/";

#endif
