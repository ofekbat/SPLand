#include "Settlement.h"
#include <iostream>
using std::to_string;

// Constructor
Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {}

// Getter for name
const string& Settlement::getName() const {
    return name;
}

// Getter for type
SettlementType Settlement::getType() const {
    return type;
}

// Convert Settlement to string
const string Settlement::toString() const {
    //TO DO:
    return "Settlement: " + name + ", Type: " + to_string(static_cast<int>(type));
}
