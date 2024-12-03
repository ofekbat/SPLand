#include "Facility.h"

//FacilityType Implementation

//Constructor
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score) {}

FacilityType::FacilityType(const FacilityType& other)
    : name(other.name),
      category(other.category),
      price(other.price),
      lifeQuality_score(other.lifeQuality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {}

//TO DO: צריך את זה? - לבדוק אם יש שימוש בזה (נראלי שהמימוש לא נכון) 
FacilityType& FacilityType::operator=(const FacilityType& other) {
    if (this != &other) {
    }
    return *this;
}

FacilityType::FacilityType(FacilityType &&other) noexcept
    : name(std::move(other.name)),
      category(other.category),
      price(other.price),
      lifeQuality_score(other.lifeQuality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {}

//TO DO: צריך את זה? - לבדוק אם יש שימוש בזה (נראלי שהמימוש לא נכון) 
FacilityType &FacilityType::operator=(FacilityType &&other) noexcept {
    if (this != &other) {
    }
    return *this;
}

//Getters
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

//Facility Implementation

//Constructor (fully initialized)
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score),
      settlementName(settlementName),
      status(FacilityStatus::UNDER_CONSTRUCTIONS),
      timeLeft(price) {}

#include "Facility.h"

// Constructor: Creates a Facility based on a FacilityType and a settlement name
Facility::Facility(const FacilityType &type, const string &settlementName)
    : FacilityType(type),                     
      settlementName(settlementName),         
      status(FacilityStatus::UNDER_CONSTRUCTIONS), 

    //TO DO: היה כאן 10. שיניתי לפרייס מהטייפ
      timeLeft(type.getCost()) {                          
}

const string& Facility::getSettlementName() const {
    return settlementName;
}

const int Facility::getTimeLeft() const {
    return timeLeft;
}

//Advance construction, update status if finished
FacilityStatus Facility::step() {
    if (timeLeft > 0) {
        timeLeft--;
        if (timeLeft == 0) {
            status = FacilityStatus::OPERATIONAL;
        }
    }
    return status;
}

void Facility::setStatus(FacilityStatus newStatus) {
    status = newStatus;
}

const FacilityStatus& Facility::getStatus() const {
    return status;
}

const string Facility::toString() const {
    return "Facility: " + getName() + "\n" +
           "Status: " + (status == FacilityStatus::UNDER_CONSTRUCTIONS ? "Under Construction" : "Operational") + "\n";
}