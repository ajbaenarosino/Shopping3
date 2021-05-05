/* 
 * File:   EventSet.cpp
 * @author MP-Team DECSAI
 * @warning Update the class. Methods read and write To be implemented by students 
 * remove rawFilters is not longer used, -> deprecated
 */


#include <unordered_map>
#include <string>
#include <fstream>
#include <cassert>
#include "DateTime.h"
#include "EventSet.h"

using namespace std;

Event NULL_EVENT = Event();

/**
 * @brief Give string[] and a string, it searches for string within string[]
 * @param array     The set of strings to look for
 * @param n_array   The size of string[]
 * @param name      The string that is being seeked
 * @return          The position of the first occurrence of string within string[]
 *  -1 if the string does not belong to string[]
 */
int findString(const string array[], int n_array, const string &name) {
    int pos = -1;
    for (int i = 0; i < n_array && pos == -1; i++) {
        if (array[i] == name) {
            pos = i;
        }
    }
    return pos;
}

std::string EventSet::to_string() const {
    string result;
    result = std::to_string(_nEvents) + " ";
    for (int i = 0; i < _nEvents; i++)
        result += _events[i].to_string() + "\n";
    return result;
}

EventSet::EventSet() : _nEvents(0) {
}

int EventSet::size() const {
    return _nEvents;
}

void EventSet::clear() {
    _nEvents = 0;
}

int EventSet::add(const Event & e) {
    int x = 0;
    if (_nEvents < MAXEVENT) {
        _events[_nEvents] = e;
        _nEvents++;
        x = 1;
    }
    return x;
}

int EventSet::add(const std::string& line) {
    Event e(line);
    return add(e);
}

const Event EventSet::at(const int pos) const {
    Event e;
    if (0 <= pos && pos < _nEvents) {
        e.set(_events[pos].to_string());
    }
    return e;
}

EventSet rawFilterEvents(const EventSet& es, const std::string &field, const std::string &value) {
    EventSet e_out;
    for (int i = 0; i < es.size(); i++) {
        DateTime aux = es.at(i).getDateTime();
        if (es.at(i).getField(field) == value || aux.NonHourDate() == value && field == "DateTime") {
            e_out.add(es.at(i));
        }
    }
    
    return e_out;
}

void findUnique(const EventSet& es, const std::string &field, std::string values[], int &n_values) {
    n_values = 0;
    if (field == "DateTime") { // I can't use findString() to find the differents values for DateTime due to needing processing it as a DateTime variable in order to use NonHourDate()
        for (int i = 0; i < es.size(); i++) {
            DateTime aux_1(es.at(i).getDateTime());
            bool isRepeated = false;

            for (int j = 0; j < n_values && !isRepeated; j++) {
                DateTime aux_2(es.at(j).getDateTime());
                if (aux_1.sameDay(aux_2)) {
                    isRepeated = true;
                }
            }

            if (!isRepeated) {
                values[n_values] = aux_1.NonHourDate();
                n_values++;
            }
        }
    } else {
        for (int i = 0; i < es.size(); i++) {
            bool isRepeated = false;

            if (findString(values, n_values, es.at(i).getField(field)) != -1) {
                isRepeated = true;
            }

            if (!isRepeated) {
                values[n_values] = es.at(i).getField(field);
                n_values++;
            }
        }
    }

}

float sumPrice(const EventSet & evSet) {
    float sum = 0;
    for (int i = 0; i < evSet.size(); i++) {
        sum = sum + evSet.at(i).getPrice();
    }
    return sum;
}

void EventSet::write(ofstream &os) {
    if (os.is_open()) {
        int pos = 0;
        bool found = false;
        string line = to_string();
        for (int i = 0; i < line.size() && !found; i++) {
            if (line[i] == ' ') {
                pos = i + 1;
                found = true;
            }
        }
        os << line.substr(pos);
    }
}

bool EventSet::read(ifstream &is, const int nelements) {
    if (is.is_open()) {
        is.ignore(20, '\n');
        for (int i = 0; i < nelements; i++) {
            string line;
            getline(is, line);
            add(line);
        }
    }
}


