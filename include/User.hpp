#ifndef USER_H
#define USER_H

#include <initializer_list>
#include <string>

using std::string;

class User {
public:
  /**
  * @brief the default constructor
  */
  User() = default;

  /**
  * constructor with arguments
  */
  User(const string &t_userName, const string &t_userPassword,
       const string &t_userEmail, const string &t_userPhone);

  /**
  * @brief copy constructor
  */
  User(const User &t_user);

  /**
  * @brief get the name of the user
  * @return   return a string indicate the name of the user
  */
  string getName() const;

  /**
  * @brief set the name of the user
  * @param   a string indicate the new name of the user
  */
  void setName(const string &t_name);

  /**
  * @brief get the password of the user
  * @return   return a string indicate the password of the user
  */
  string getPassword() const;

  /**
  * @brief set the password of the user
  * @param   a string indicate the new password of the user
  */
  void setPassword(const string &t_password);

  /**
  * @brief get the email of the user
  * @return   return a string indicate the email of the user
  */
  string getEmail() const;

  /**
  * @brief set the email of the user
  * @param   a string indicate the new email of the user
  */
  void setEmail(const string &t_email);

  /**
  * @brief get the phone of the user
  * @return   return a string indicate the phone of the user
  */
  string getPhone() const;

  /**
  * @brief set the phone of the user
  * @param   a string indicate the new phone of the user
  */
  void setPhone(const string &t_phone);

private:
  string m_name;
  string m_password;
  string m_email;
  string m_phone;
};

#endif
