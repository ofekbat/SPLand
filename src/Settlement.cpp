#include "Settlement.h"
#include <iostream>
using std::to_string;

//Constructor
Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {}

//Getters
const string& Settlement::getName() const {
    return name;
}

SettlementType Settlement::getType() const {
    return type;
}

const string Settlement::toString() const {
    //TO DO:
    return "Settlement: " + name + ", Type: " + to_string(static_cast<int>(type));
}
