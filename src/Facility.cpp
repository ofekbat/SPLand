#include "Facility.h"

// FacilityType Implementation

// Constructor
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score) {}

// Getters
const string& FacilityType::getName() const {
    return name;
}

int FacilityType::getCost() const {
    return price;
}

int FacilityType::getLifeQualityScore() const {
    return lifeQuality_score;
}

int FacilityType::getEnvironmentScore() const {
    return environment_score;
}

int FacilityType::getEconomyScore() const {
    return economy_score;
}

FacilityCategory FacilityType::getCategory() const {
    return category;
}

// Facility Implementation

// Constructor (fully initialized)
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
      settlementName(settlementName),
      status(FacilityStatus::UNDER_CONSTRUCTIONS),
    //TO DO:
      timeLeft(price) {}

// Constructor (based on FacilityType)
Facility::Facility(FacilityType &type, const string &settlementName)
    : FacilityType(type),
      settlementName(settlementName),
      status(FacilityStatus::UNDER_CONSTRUCTIONS),
    //TO DO:
      timeLeft(type.getCost()) {}

// Get the name of the settlement
const string& Facility::getSettlementName() const {
    return settlementName;
}

// Get the timeLeft
const int Facility::getTimeLeft() const {
    return timeLeft;
}

// Advance construction, update status if finished
FacilityStatus Facility::step() {
    if (timeLeft > 0) {
        timeLeft--;
        if (timeLeft == 0) {
            status = FacilityStatus::OPERATIONAL;
        }
    }
    return status;
}

// Set the status
void Facility::setStatus(FacilityStatus newStatus) {
    status = newStatus;
}

// Get the current status
const FacilityStatus& Facility::getStatus() const {
    return status;
}

// Convert the facility to a string
const string Facility::toString() const {
    return "Facility: " + getName() +
           ", Settlement: " + settlementName +
           ", Status: " + (status == FacilityStatus::UNDER_CONSTRUCTIONS ? "Under Construction" : "Operational") +
           ", Time Left: " + std::to_string(timeLeft) +
           ", Category: " + std::to_string(static_cast<int>(getCategory()));
}
