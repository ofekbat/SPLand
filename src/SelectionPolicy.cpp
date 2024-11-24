#include "SelectionPolicy.h"

NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex++;
    if (lastSelectedIndex >= facilitiesOptions.size()) {
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
        int maxScore = std::max({
            LifeQualityScore + facility.getLifeQualityImpact(),
            EconomyScore + facility.getEconomyImpact(),
            EnvironmentScore + facility.getEnvironmentImpact()
        });

        int minScore = std::min({
            LifeQualityScore + facility.getLifeQualityImpact(),
            EconomyScore + facility.getEconomyImpact(),
            EnvironmentScore + facility.getEnvironmentImpact()
        });

        int difference = maxScore - minScore;
        if (difference < minDifference) {
            minDifference = difference;
            bestFacility = &facility;
        }
    }

    if (!bestFacility) {
        static FacilityType defaultFacility;
        return defaultFacility;
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
    int size = facilitiesOptions.size();
    for (int i = 0; i < size; ++i) {
        int index = (lastSelectedIndex + i) % size;
        if (facilitiesOptions[index].getCategory() == FacilityCategory::Economy) {
            lastSelectedIndex = index;
            return facilitiesOptions[index];
        }
    }
    static FacilityType defaultFacility;
    return defaultFacility;
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
    int size = facilitiesOptions.size();
    for (int i = 0; i < size; ++i) {
        int index = (lastSelectedIndex + i) % size;
        if (facilitiesOptions[index].getCategory() == FacilityCategory::Environment) {
            lastSelectedIndex = index;
            return facilitiesOptions[index];
        }
    }
    static FacilityType defaultFacility;
    return defaultFacility;
}

const string SustainabilitySelection::toString() const {
    return "SustainabilitySelection";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}
