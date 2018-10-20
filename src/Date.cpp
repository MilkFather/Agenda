#include "Date.hpp"
#include <sstream>
#include <cstdio>
#include <exception>

Date::Date() {
    this->m_year = 0;
    this->m_month = 0;
    this->m_day = 0;
    this->m_hour = 0;
    this->m_minute = 0;
}

Date::Date(int t_year, int t_month, int t_day, int t_hour, int t_minute) {
    this->m_year = t_year;
    this->m_month = t_month;
    this->m_day = t_day;
    this->m_hour = t_hour;
    this->m_minute = t_minute;
}

Date::Date(const std::string &dateString) {
    if (dateString.length() != 16) {
        this->m_year = 0;
        this->m_month = 0;
        this->m_day = 0;
        this->m_hour = 0;
        this->m_minute = 0;
    } else if (dateString[4] != '-' ||
               dateString[7] != '-' ||
               dateString[10] != '/' ||
               dateString[13] != ':') {
        this->m_year = 0;
        this->m_month = 0;
        this->m_day = 0;
        this->m_hour = 0;
        this->m_minute = 0;
    } else {
        try {
            this->m_year = std::stoi(dateString.substr(0, 4));
            this->m_month = std::stoi(dateString.substr(5, 2));
            this->m_day = std::stoi(dateString.substr(8, 2));
            this->m_hour = std::stoi(dateString.substr(11, 2));
            this->m_minute = std::stoi(dateString.substr(14, 2));
        } catch (std::exception e) {
            this->m_year = 0;
            this->m_month = 0;
            this->m_day = 0;
            this->m_hour = 0;
            this->m_minute = 0;
        }
    }
}

int Date::getYear(void) const {
    return this->m_year;
}

void Date::setYear(const int t_year) {
    if (this->m_year != t_year)
        this->m_year = t_year;
}

int Date::getMonth(void) const {
    return this->m_month;
}

void Date::setMonth(const int t_month) {
    if (this->m_month != t_month)
        this->m_month = t_month;
}

int Date::getDay(void) const {
    return this->m_day;
}

void Date::setDay(const int t_day) {
    if (this->m_day != t_day)
        this->m_day = t_day;
}

int Date::getHour(void) const {
    return this->m_hour;
}

void Date::setHour(const int t_hour) {
    if (this->m_hour != t_hour)
        this->m_hour = t_hour;
}

int Date::getMinute(void) const {
    return this->m_minute;
}

void Date::setMinute(const int t_minute) {
    if (this->m_minute != t_minute)
        this->m_minute = t_minute;
}

bool Date::isValid(const Date &t_date) {
    int y = t_date.m_year;
    bool leap = ((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0);
    int days[13] = {-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int days_l[13] = {-1, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (t_date.m_year < 1000 || t_date.m_year > 9999)
        return false;
    if (t_date.m_month < 1 || t_date.m_month > 12)
        return false;
    if (t_date.m_day < 1)
        return false;
    if ((leap && t_date.m_day > days_l[t_date.m_month]) ||
        (!leap && t_date.m_day > days[t_date.m_month]))
        return false;
    if (t_date.m_hour < 0 || t_date.m_hour > 23)
        return false;
    if (t_date.m_minute < 0 || t_date.m_minute > 59)
        return false;

    return true;
}

Date Date::stringToDate(const std::string &t_dateString) {
    Date d;
    if (t_dateString.length() != 16)
        return d;
    if (t_dateString[4] != '-' ||
        t_dateString[7] != '-' ||
        t_dateString[10] != '/' ||
        t_dateString[13] != ':')
        return d;
    try {
        d.m_year = stoi(t_dateString.substr(0, 4));
        d.m_month = stoi(t_dateString.substr(5, 2));
        d.m_day = stoi(t_dateString.substr(8, 2));
        d.m_hour = stoi(t_dateString.substr(11, 2));
        d.m_minute = stoi(t_dateString.substr(14, 2));
        return d;
    } catch (std::exception e) {
        Date f;
        return f;
    }
}

std::string Date::dateToString(const Date &t_date) {
    if (Date::isValid(t_date)) {
        char str[17];
        snprintf(str, sizeof(str),
                 "%04d-%02d-%02d/%02d:%02d",
                 t_date.m_year, t_date.m_month,
                 t_date.m_day, t_date.m_hour, t_date.m_minute);

        std::string s(str);
        return s;
    } else {
        return "0000-00-00/00:00";
    }
}

Date& Date::operator=(const Date &t_date) {
    this->m_year = t_date.m_year;
    this->m_month = t_date.m_month;
    this->m_day = t_date.m_day;
    this->m_hour = t_date.m_hour;
    this->m_minute = t_date.m_minute;

    return *this;
}

bool Date::operator==(const Date &t_date) const {
    return
    (this->m_year == t_date.m_year) &&
    (this->m_month == t_date.m_month) &&
    (this->m_day == t_date.m_day) &&
    (this->m_hour == t_date.m_hour) &&
    (this->m_minute == t_date.m_minute);
}

bool Date::operator>(const Date &t_date) const {
    if (this->m_year > t_date.m_year) {
        return true;
    } else if (this->m_year < t_date.m_year) {
        return false;
    } else {
        if (this->m_month > t_date.m_month) {
            return true;
        } else if (this->m_month < t_date.m_month) {
            return false;
        } else {
            if (this->m_day > t_date.m_day) {
                return true;
            } else if (this->m_day < t_date.m_day) {
                return false;
            } else {
                if (this->m_hour > t_date.m_hour) {
                    return true;
                } else if (this->m_hour < t_date.m_hour) {
                    return false;
                } else {
                    return this->m_minute > t_date.m_minute;
                }
            }
        }
    }
}

bool Date::operator<(const Date &t_date) const {
    if (this->m_year < t_date.m_year) {
        return true;
    } else if (this->m_year > t_date.m_year) {
        return false;
    } else {
        if (this->m_month < t_date.m_month) {
            return true;
        } else if (this->m_month > t_date.m_month) {
            return false;
        } else {
            if (this->m_day < t_date.m_day) {
                return true;
            } else if (this->m_day > t_date.m_day) {
                return false;
            } else {
                if (this->m_hour < t_date.m_hour) {
                    return true;
                } else if (this->m_hour > t_date.m_hour) {
                    return false;
                } else {
                    return this->m_minute < t_date.m_minute;
                }
            }
        }
    }
}

bool Date::operator>=(const Date &t_date) const {
    return !((*this) < t_date);
}

bool Date::operator<=(const Date &t_date) const {
    return !((*this) > t_date);
}
