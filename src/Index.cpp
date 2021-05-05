/* 
 * File:   Index.cpp
 * @author MP-Team DECSAI
 * @warning To be implemented by students 
 */
#include <string>
#include <iostream>
#include <cassert>
#include <fstream>
#include "Index.h"

using namespace std;

Index::Index() : _nEntries(0) {
}

Index::Index(const int onBrand) {
    _nEntries = 0;
    setIOnWhich(onBrand);
}

void Index::setIOnWhich(const int val) {
    _onBrand = val;
}

int Index::size() const {
    return _nEntries;
}

void Index::clear() {
    _nEntries = 0;
    for (int i = 0; i < MAXEVENT; i++) {
        Pair p;
        _entries[i] = p;
    }
}

int Index::getIOnWhich() const {
    return _onBrand;
}

int Index::add(const Pair &pair) {
    int added = 0;
    if (!pair.isEmpty()) {
        int pos = upper_bound(pair.getKey());
        for (int i = _nEntries; i > pos; i--) {
            _entries[i] = _entries[i - 1];
        }
        _entries[pos] = pair;
        _nEntries++;
        added = 1;
    }
    return added;
}

/**
 * @brief Build the index using the Brand o the userID as searching key
 * @param evSet origin
 */
void Index::build(const EventSet &evSet, const int onBrand) {
    clear();
    _onBrand = onBrand;
    Pair p;
    for (int i = 0; i < evSet.size(); i++) {
        if (_onBrand == 0) {
            p.set(evSet.at(i).getUserID(), i);
        } else if (_onBrand == 1) {
            p.set(evSet.at(i).getBrand(), i);
        }
        if (!p.isEmpty()) {
            add(p);
        }
    }
}

/**
 * @brief Returns the position to the first element in the Index
 * which is not considered to go before key (i.e., either it is equivalent is the first one
 * or it goes after), look at the example.
 * @param key the searching key a string
 * example: // 10 20 30 30 40 50 60 70
 * low_bound(30) ....^
 * low_bound(35) ..........^
 * @return a position in the Index
 */
int Index::lower_bound(const string &key) const {
    int pos = 0;
    int left = 0, right = _nEntries - 1, curr_pos;
    bool found_1 = false;
    while (left <= right && !found_1) {
        curr_pos = (left + right) / 2;
        if (_entries[curr_pos].getKey() == key) {
            pos = curr_pos;
            found_1 = true;
        } else if (key < _entries[curr_pos].getKey()) {
            right = curr_pos - 1;
        } else if (key > _entries[curr_pos].getKey()) {
            left = curr_pos + 1;
        } else {
            left++;
        }
    }
    if (found_1) {
        bool found_2 = false;
        for (int i = pos; i >= 0 && !found_2; i--) {
            if (_entries[i].getKey() != key) {
                pos = i + 1;
                found_2 = true;
            }
        }
        if (!found_2) {
            pos = 0;
        }
    } else {
        pos = right;
    }
    return pos;
}
// Un cambio xd

/**
 * @brief Returns the position to the first element in the Index which is considered to go after val.
 * look at the example.
 * example: // 10 20 30 30 40 50 60 70
 * upper_bound(30) ........^
 * upper_bound(35) ........^
 * @param key the searching key a string
 * @return a position in the Index
 */
int Index::upper_bound(const string &key) const {
    int pos = 0;
    int left = 0, right = _nEntries - 1, curr_pos;
    bool found_1 = false;
    while (left <= right && !found_1) {
        curr_pos = (left + right) / 2;
        if (_entries[curr_pos].getKey() == key) {
            pos = curr_pos;
            found_1 = true;
        } else if (key < _entries[curr_pos].getKey()) {
            right = curr_pos - 1;
        } else if (key > _entries[curr_pos].getKey()) {
            left = curr_pos + 1;
        } else {
            right--;
        }
    }
    if (found_1) {
        bool found_2 = false;
        for (int i = pos; i < _nEntries && !found_2; i++) {
            if (_entries[i].getKey() != key) {
                pos = i;
                found_2 = true;
            }
        }
        if (!found_2) {
            pos = _nEntries;
        }
    } else {
        pos = left;
    }
    return pos;
}

const Pair Index::at(const int pos) const {
    Pair p;
    if (0 <= pos && pos < _nEntries) {
        p = _entries[pos];
    }
    return p;
}

Pair Index::at(int pos) {
    Pair p;
    if (0 <= pos && pos < _nEntries) {
        p = _entries[pos];
    }
    return p;
}

void Index::print() const {
    //
}

// external functions

Event getEvent(const EventSet &evSet, const Index &indx, const int pos) {
    return evSet.at(indx.at(pos).getPos());
}

void write(ofstream &os, const EventSet &evSet, const Index &indx) {
    if (os.is_open()) {
        for (int i = 0; i < indx.size(); i++) {
            os << getEvent(evSet, indx, i).to_string() << endl;
        }
    } else {
        os << "Error in call" << endl;
    }
}

Index rawFilterIndex(const EventSet &evSet, const Index &indx, const std::string &field, const std::string &value) {
    Index out_indx(indx.getIOnWhich());
    if (field == "UserID" && indx.getIOnWhich() == 0 || field == "Brand" && indx.getIOnWhich() == 1) {
        int left = indx.lower_bound(value), right = indx.upper_bound(value);
        for (int i = left; i < right; i++) {
            Pair p(getEvent(evSet, indx, i).getField(field), indx.at(i).getPos());
            out_indx.add(p);
        }
    } else {
        for (int i = 0; i < evSet.size(); i++) {
            DateTime aux(value);
            if (evSet.at(i).getField(field) == value || evSet.at(i).getDateTime().NonHourDate() == aux.NonHourDate() && field == "DateTime") {
                Pair p;
                if (indx.getIOnWhich() == 1) {
                    p.set(evSet.at(i).getBrand(), i);
                } else {
                    p.set(evSet.at(i).getUserID(), i);
                }
                out_indx.add(p);
            }
        }
    }
    return out_indx;
}

float sumPrice(const EventSet &evSet, const Index &indx) {
    float sum = 0;
    for (int i = 0; i < indx.size(); i++) {
        sum += getEvent(evSet, indx, i).getPrice();
    }
    return sum;
}