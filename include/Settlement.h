#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

class Facility;

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
};

class Settlement {
public:
    Settlement(const string &name, SettlementType type);
    Settlement(const Settlement& other);  // Copy constructor
    const string &getName() const;
    SettlementType getType() const;
    const string toString() const;
    const int getConstructionLimit() const;

private:
    const string name;
    SettlementType type;
};
