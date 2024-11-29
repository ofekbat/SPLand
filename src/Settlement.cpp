#include "Settlement.h"
#include <utility>

//constractor
Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {}

//copy constructor
Settlement::Settlement(const Settlement &other)
    : name(other.name), type(other.type) {}

//move constructor
Settlement::Settlement(Settlement &&other)
    : name(std::move(other.name)), type(other.type) {}

//destractor
Settlement::~Settlement() {}

//getters
const string &Settlement::getName() const {
    return name;
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
