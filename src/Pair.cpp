/* 
 * File:   Pair.cpp
 * @author MP-Team DECSAI
 * @warning To be implemented by students
 */
#include <string>

#include "Pair.h"

using namespace std;

Pair NULL_PAIR = Pair();

Pair::Pair() {
    initDefault();
}

bool Pair::isEmpty() const {
    return _key == EMPTY_FIELD && _pos == -1;
}

Pair::Pair(const std::string &key, const int pos) {
    set(key, pos);
}

std::string Pair::getKey() const {
    return _key;

}

int Pair::getPos() const {
    return _pos;
}

void Pair::setKey(const std::string &key) {
    if (key == "") {
        initDefault();
    } else {
        _key = key;
    }
}

void Pair::setPos(const int pos) {
    if (pos >= -1) {
        _pos = pos;
    } else {
        _pos = -1;
    }
}

void Pair::set(const std::string &key, const int pos) {
    setPos(pos);
    setKey(key);
}

std::string Pair::to_string() const {
    return _key + " " + std::to_string(_pos);
}

void Pair::initDefault() {
    set(EMPTY_FIELD, -1);
}

bool equalKey(const Pair &left, const Pair &right) {
    return left.getKey() == right.getKey();
}
