#include "Settlement.h"
#include <iostream>
using std::to_string;

//constructor
Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {}

//getters
const string& Settlement::getName() const {
    return name;
}

SettlementType Settlement::getType() const {
    return type;
}

const std::string Settlement::toString() const {
    std::string c;
    
    if (static_cast<int>(type) == 0) {
        c = "VILLAGE";
    } else if (static_cast<int>(type) == 1) {
        c = "CITY";
    } else{
        c = "METROPOLIS";
    }

    return "Settlement: " + name + ", Type: " + c;
}
