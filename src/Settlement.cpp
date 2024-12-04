#include "Settlement.h"
#include <utility>
#include <iostream>

//constractor
Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {}

//getters
const string &Settlement::getName() const {
    return name;
}

Settlement::Settlement(const Settlement& other)
    : name(other.name), type(other.type) {
}

SettlementType Settlement::getType() const {
    return type;
}

const int Settlement::getConstructionLimit() const{
    if (type == SettlementType::VILLAGE) {
        return 1;
    } else if (type == SettlementType::CITY) {
        return 2;
    } else {
        return 3;
    }
}

const string Settlement::toString() const {
    string c;
    if (type == SettlementType::VILLAGE) {
        c = "VILLAGE";
    } else if (type == SettlementType::CITY) {
        c = "CITY";
    } else {
        c = "METROPOLIS";
    }
    return "Settlement: " + name + ", Type: " + c;
}
