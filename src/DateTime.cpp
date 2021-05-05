/**
 * @file DateTime.cpp
 * @author DECSAI
 * @note To be implemented by students 
 * 
 */

#include <iostream>
#include <sstream>
#include <valarray>
#include "DateTime.h"
using namespace std;

/**
 * @brief auxiliary function to check if the date and the time are in correct 
 * format, taking into account, leap year etc.
 * @param year input
 * @param month input
 * @param day input
 * @param hour input
 * @param min input
 * @param sec input
 * @return true if is correct, false if not
 */
bool isCorrect(int year, int month, int day, int hour, int min, int sec);
/**
 * @brief split the first field in 6 components of the data time.
 * Please consider using string::substr(int, int)[https://en.cppreference.com/w/cpp/string/basic_string/substr] to cut the line
 * into the appropriate substrings and then convert it into integer values with 
 * the function stoi(string) [https://en.cppreference.com/w/cpp/string/basic_string/stol]
 * ~~~~
 *      YYYY-MM-dd hh:mm:ss UTC 
 *      +----+----+----+----+----+ 
 *      |    |    |    |    |    |
 *      0    5    10   15   20   25
 * ~~~~
 *  * @param line input string
 * @param y output int
 * @param m output int
 * @param d output int
 * @param h output int
 * @param mn output int
 * @param s output int
 */
void split(const std::string &line, int &y, int &m, int &d, int &h, int &mn, int &s);

DateTime::DateTime() {
    initDefault();
}

void DateTime::initDefault() {
    set(DATETIME_DEFAULT);
}

bool isCorrect(int year, int month, int day, int hour, int min, int sec) {
    bool correct;
    int lastDay = 31;
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        lastDay = 30;
    } else if (month == 2) {
        lastDay = 28;
        if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) { //Taking in count leap-years
            lastDay = 29;
        }
    }
    correct = (month >= 1 && month <= 12) && (day > 0 && day <= lastDay) && (0 <= hour && hour < 24) && (0 <= min && min < 60) && (0 <= sec && sec < 60);
    return correct;
}

void split(const std::string &line, int &y, int &m, int &d, int &h, int &mn, int &s) {
    try {
        y = stoi(line.substr(0, 4));
        m = stoi(line.substr(5, 2));
        d = stoi(line.substr(8, 2));
        h = stoi(line.substr(11, 2));
        mn = stoi(line.substr(14, 2));
        s = stoi(line.substr(17, 2));
    } catch (...) { //If the function cannot split the string correctly, it sets a default date
        y = 1971;
        m = 1;
        d = 1;
        h = 0;
        mn = 0;
        s = 0;
    }
}

void DateTime::set(const std::string &line) {
    int year, month, day, hour, min, sec;
    split(line, year, month, day, hour, min, sec);
    if (isCorrect(year, month, day, hour, min, sec)) {
        _year = year;
        _month = month;
        _day = day;
        _hour = hour;
        _min = min;
        _sec = sec;
    } else {
        _year = 1971;
        _month = 1;
        _day = 1;
        _hour = 0;
        _min = 0;
        _sec = 0;
    }
}

DateTime::DateTime(const string &line) {
    set(line);
}

int DateTime::year() const {
    return _year;
}

int DateTime::month() const {
    return _month;
}

int DateTime::day() const {
    return _day;
}

int DateTime::hour() const {
    return _hour;
}

int DateTime::min() const {
    return _min;
}

int DateTime::sec() const {
    return _sec;
}

bool DateTime::isBefore(const DateTime &one) const {
    int _aux[] = {_year, _month, _day, _hour, _min, _sec};
    int auxone[] = {one.year(), one.month(), one.day(), one.hour(), one.min(), one.sec()};
    
    bool isBefore = false, assigned = false;
    for (int i = 0; i < 6 && assigned == false; i++) {  //This algorithm compares each date following a date hierarchy (first it compares
        if (_aux[i] < auxone[i]) {                      //the year, and if they are equal, it continues comparing lower hierarchy values
            isBefore = true;                            //(month in this case) until it finds a difference.
            assigned = true;                            //If both dates are equal it returns false.
        } else if (_aux[i] > auxone[i]) {
            isBefore = false;
            assigned = true;
        }
    }
    
    return isBefore;
}

int DateTime::weekDay() const {
    int q = _day;
    int y = _year;
    int m = _month;
    if (m < 3) {
        m += 12;
        y--;
    }
    return ((q + (31 * ((m - 2)) / 12) + y + (y / 4) - (y / 100) + (y / 400)) % 7); //Simplified Zeller Congruence with J R Stockton modification: Converts a date into a weekday, beeing sunday = 0... saturday = 6
}

bool DateTime::sameDay(DateTime other) {
    bool sameDay = false;
    if (year() == other.year() && month() == other.month() && day() == other.day()) {
        sameDay = true;
    }
    return sameDay;
}

string DateTime::to_string() const {
    char local[64];
    sprintf(local, "%04i-%02i-%02i %02i:%02i:%02i UTC", _year, _month, _day, _hour, _min, _sec);
    return local;
}

string DateTime::NonHourDate() const {
    char local[64];
    sprintf(local, "%04i-%02i-%02i 00:00:00 UTC", _year, _month, _day);
    return local;
}