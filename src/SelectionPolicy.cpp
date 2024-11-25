#include "SelectionPolicy.h"
#include <stdexcept>
#include <climits>

NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex++;
    if (lastSelectedIndex >= static_cast<int>(facilitiesOptions.size())) {
        lastSelectedIndex = 0;
    }
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const {
    return "NaiveSelection";
}

NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}

BalancedSelection::BalancedSelection(int lifeQ, int eco, int env)
    : LifeQualityScore(lifeQ), EconomyScore(eco), EnvironmentScore(env) {}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    int minDifference = INT_MAX;
    const FacilityType* bestFacility = nullptr;
for (const auto& facility : facilitiesOptions) {
    int lifeQualityScore = LifeQualityScore + facility.getLifeQualityScore();
    int economyScore = EconomyScore + facility.getEconomyScore();
    int environmentScore = EnvironmentScore + facility.getEnvironmentScore();

    int maxScore = std::max(std::max(lifeQualityScore, economyScore), environmentScore);
    int minScore = std::min(std::min(lifeQualityScore, economyScore), environmentScore);

    int difference = maxScore - minScore;

    if (difference < minDifference) {
        minDifference = difference;
        bestFacility = &facility;
    }
}
    return *bestFacility;
}

const string BalancedSelection::toString() const {
    return "BalancedSelection";
}

BalancedSelection* BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}

EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

//TO DO:
const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
<<<<<<< HEAD
    for (int i = 0; i < static_cast<int>(facilitiesOptions.size()); ++i) {
        int index = (lastSelectedIndex + 1 + i) % facilitiesOptions.size();
        if (facilitiesOptions[index].getCategory() == FacilityCategory::ECONOMY) {
=======
    int size = facilitiesOptions.size();
    for (int i = 0; i < size; ++i) {
        int index = (lastSelectedIndex + i) % size;
        if (facilitiesOptions[index].getCategory() == FacilityCategory::Economy) {
>>>>>>> bbd2ae4d78eeeee08521b1cd52b66179f8708bcb
            lastSelectedIndex = index;
            return facilitiesOptions[index];
        }
    }
    //TO DO:
    throw std::runtime_error("No facility found"); // ברירת מחדל אם אי אפשר להחזיר ערך
}

const string EconomySelection::toString() const {
    return "EconomySelection";
}

EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this);
}

SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

//TO DO:
const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
<<<<<<< HEAD
    for (int i = 0; i < static_cast<int>(facilitiesOptions.size()); ++i) {
        int index = (lastSelectedIndex + 1 + i) % facilitiesOptions.size();
        if (facilitiesOptions[index].getCategory() == FacilityCategory::ENVIRONMENT) {
=======
    int size = facilitiesOptions.size();
    for (int i = 0; i < size; ++i) {
        int index = (lastSelectedIndex + i) % size;
        if (facilitiesOptions[index].getCategory() == FacilityCategory::Environment) {
>>>>>>> bbd2ae4d78eeeee08521b1cd52b66179f8708bcb
            lastSelectedIndex = index;
            return facilitiesOptions[index];
        }
    }
    throw std::runtime_error("No facility found"); // ברירת מחדל אם אי אפשר להחזיר ערך
}

const string SustainabilitySelection::toString() const {
    return "SustainabilitySelection";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}
