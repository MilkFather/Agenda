#include "User.hpp"

User::User(std::string t_userName,
           std::string t_userPassword,
           std::string t_userEmail, std::
           string t_userPhone) {
    this->m_name = t_userName;
    this->m_password = t_userPassword;
    this->m_email = t_userEmail;
    this->m_phone = t_userPhone;
}

User::User(const User &t_user) {
    this->m_name = t_user.m_name;
    this->m_password = t_user.m_password;
    this->m_email = t_user.m_email;
    this->m_phone = t_user.m_phone;
}

std::string User::getName() const {
    return this->m_name;
}

void User::setName(std::string t_name) {
    if (this->m_name != t_name)
        this->m_name = t_name;
}

std::string User::getPassword() const {
    return this->m_password;
}

void User::setPassword(std::string t_password) {
    if (this->m_password != t_password)
        this->m_password = t_password;
}

std::string User::getEmail() const {
    return this->m_email;
}

void User::setEmail(std::string t_email) {
    if (this->m_email != t_email)
        this->m_email = t_email;
}

std::string User::getPhone() const {
    return this->m_phone;
}

void User::setPhone(std::string t_phone) {
    if (this->m_phone != t_phone)
        this->m_phone = t_phone;
}
