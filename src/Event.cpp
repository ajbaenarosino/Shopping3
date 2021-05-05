/**
 * @file Event.cpp
 * @author MP-Team DECSAI
 * @warning Update the class. Methods print and read To be implemented by students 
 * 
 */

#include "Event.h"
using namespace std;

Event::Event() {
    initDefault();
}

void Event::initDefault() {
    set(EVENT_DEFAULT);
}

Event::Event(const string &line) {
    set(line);
}

DateTime Event::getDateTime() const {
    return _dateTime;
}

string Event::getType() const {
    return _type;
}

string Event::getProductID() const {
    return _prod_id;
}

string Event::getCategoryID() const {
    return _cat_id;
}

string Event::getCategoryCode() const {
    return _cat_cod;
}

string Event::getBrand() const {
    return _brand;
}

double Event::getPrice() const {
    return _price;
}

string Event::getUserID() const {
    return _user_id;
}

string Event::getSession() const {
    return _session;
}

void Event::setDateTime(string const &time) {
    _dateTime.set(time);
}

void Event::setType(std::string const &type) {
    bool valid = false;
    for (int i = 0; i < sizeof (VALID_TYPES) / sizeof (string) && valid == false; i++) { // Checks if the input type is among the valid types.
        if (type == VALID_TYPES[i]) {
            valid = true;
        }
    }
    if (valid) {
        _type = type;
    } else {
        _type = VALID_TYPES[0];
    }
}

void Event::setProductID(std::string const &prod_id) {
    if (prod_id != "") {
        _prod_id = prod_id;
    } else {
        _prod_id = EMPTY_FIELD;
    }
}

void Event::setCategoryID(std::string const &cat_id) {
    _cat_id = cat_id;
}

void Event::setCategoryCode(std::string const &cat_cod) {
    _cat_cod = cat_cod;
}

void Event::setBrand(std::string const &brand) {
    _brand = brand;
}

void Event::setPrice(const double price) {
    if (price >= 0) {
        _price = price;
    } else {
        _price = -1.0;
    }
}

void Event::setPrice(const std::string price) { //Added this function to not need to make the conversion from string to double before calling the function
    try {
        setPrice(stod(price));
    } catch (...) {
        setPrice(-1.0);
    }
}

void Event::setUserID(std::string const &user_id) {
    if (user_id != "") {
        _user_id = user_id;
    } else {
        _user_id = EMPTY_FIELD;
    }
}

void Event::setSession(std::string const &session) {
    if (session != "") {
        _session = session;
    } else {
        _session = EMPTY_FIELD;
    }
}

void Event::set(std::string const &line) {
    string aux[9];

    for (int i = 0, j = 0; i < 9; i++) {
        aux[i] = line.substr(j, line.find(",", j) - j);
        j = line.find(",", j) + 1;
    }

    setDateTime(aux[0]);
    setType(aux[1]);
    setProductID(aux[2]);
    setCategoryID(aux[3]);
    setCategoryCode(aux[4]);
    setBrand(aux[5]);
    setPrice(aux[6]);
    setUserID(aux[7]);
    setSession(aux[8]);
}

string Event::to_string() const {
    string salida;
    salida += _dateTime.to_string();
    salida += "," + _type;
    salida += "," + _prod_id;
    salida += "," + _cat_id;
    salida += "," + _cat_cod;
    salida += "," + _brand;
    salida += "," + std::to_string(_price);
    salida += "," + _user_id;
    salida += "," + _session;
    //    salida = salida +"," + DAYNAME[this->getDateTime().weekDay()];
    return salida;
}

bool Event::isEmpty() const {
    return _prod_id == EMPTY_FIELD || _user_id == EMPTY_FIELD || _session == EMPTY_FIELD || _dateTime.to_string() == DATETIME_DEFAULT;
}

string Event::getField(const string &field) const {
    string s, event = to_string();
    for (int i = 0; i < sizeof (VALID_FIELDS) / sizeof (*VALID_FIELDS); i++) {
        if (field == VALID_FIELDS[i]) {
            int x = 0;
            for (int j = 0; j < i; j++) {
                x = event.find(',', x) + 1;
            }
            s = event.substr(x, event.find(',', x) - x);
        }
    }
    return s;
}

void Event::write(ofstream &os) {
    if (os.is_open()) {
        os << to_string();
    }
}

void Event::read(ifstream &is) {
    if (is.is_open()) {
        string line;
        getline(is, line);
        set(line);  
    }
}
