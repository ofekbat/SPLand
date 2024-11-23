#include "SelectionPolicy.h"
#include <stdexcept>
#include <limits>
#include <sstream>

// NaiveSelection implementation
NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::runtime_error("No facilities available for selection.");
    }
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

const string NaiveSelection::toString() const {
    return "NaiveSelection";
}

NaiveSelection* NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}

// BalancedSelection implementation
BalancedSelection::BalancedSelection(int lifeQ, int eco, int env)
    : LifeQualityScore(lifeQ), EconomyScore(eco), EnvironmentScore(env) {}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::runtime_error("No facilities available for selection.");
    }

    int minDifference = std::numeric_limits<int>::max();
    const FacilityType* bestFacility = nullptr;

    for (const auto& facility : facilitiesOptions) {
        int maxScore = std::max({LifeQualityScore + facility.getLifeQualityImpact(),
                                 EconomyScore + facility.getEconomyImpact(),
                                 EnvironmentScore + facility.getEnvironmentImpact()});
        int minScore = std::min({LifeQualityScore + facility.getLifeQualityImpact(),
                                 EconomyScore + facility.getEconomyImpact(),
                                 EnvironmentScore + facility.getEnvironmentImpact()});
        int difference = maxScore - minScore;

        if (difference < minDifference) {
            minDifference = difference;
            bestFacility = &facility;
        }
    }

    if (!bestFacility) {
        throw std::runtime_error("No suitable facility found.");
    }

    return *bestFacility;
}

const string BalancedSelection::toString() const {
    return "BalancedSelection";
}

BalancedSelection* BalancedSelection::clone() const {
    return new BalancedSelection(*this);
}

// EconomySelection implementation
EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    for (size_t i = 0; i < facilitiesOptions.size(); ++i) {
        size_t index = (lastSelectedIndex + 1 + i) % facilitiesOptions.size();
        if (facilitiesOptions[index].getCategory() == FacilityCategory::Economy) {
            lastSelectedIndex = index;
            return facilitiesOptions[index];
        }
    }
    throw std::runtime_error("No economy facilities available.");
}

const string EconomySelection::toString() const {
    return "EconomySelection";
}

EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this);
}

// SustainabilitySelection implementation
SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    for (size_t i = 0; i < facilitiesOptions.size(); ++i) {
        size_t index = (lastSelectedIndex + 1 + i) % facilitiesOptions.size();
        if (facilitiesOptions[index].getCategory() == FacilityCategory::Environment) {
            lastSelectedIndex = index;
            return facilitiesOptions[index];
        }
    }
    throw std::runtime_error("No sustainability facilities available.");
}

const string SustainabilitySelection::toString() const {
    return "SustainabilitySelection";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}
