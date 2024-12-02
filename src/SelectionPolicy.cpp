#include "SelectionPolicy.h"
#include <stdexcept>
#include <climits>
#include <iostream>

//NAIVE-SELECTION
NaiveSelection::NaiveSelection() : lastSelectedIndex(-1) {}

const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::runtime_error("No facilities available");
    }

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

//BALANCED-SELECTION
BalancedSelection::BalancedSelection(int lifeQ, int eco, int env)
    : LifeQualityScore(lifeQ), EconomyScore(eco), EnvironmentScore(env),
    lifeQUnderConstruction(0), ecoUnderConstruction(0), envUnderConstruction(0) {}

const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::runtime_error("No facilities available");
    }

    int minDifference = INT_MAX;
    int difference;
    const FacilityType* bestFacility = nullptr;
    int lifeQTemp = LifeQualityScore + lifeQUnderConstruction;
    int ecoTemp = EconomyScore + ecoUnderConstruction;
    int envTemp = EnvironmentScore + envUnderConstruction;

    for (const auto& facility : facilitiesOptions) {
        int lifeQualityScore = lifeQTemp + facility.getLifeQualityScore();
        int economyScore = ecoTemp + facility.getEconomyScore();
        int environmentScore = envTemp + facility.getEnvironmentScore();

        int maxScore = std::max(std::max(lifeQualityScore, economyScore), environmentScore);
        int minScore = std::min(std::min(lifeQualityScore, economyScore), environmentScore);

        difference = maxScore - minScore;

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

void BalancedSelection::updateUnderConstruction(int lifeQ, int eco, int env) {
    lifeQUnderConstruction += lifeQ;
    ecoUnderConstruction += eco;
    envUnderConstruction += env;
}

void BalancedSelection::updateMainScores(int lifeQ, int eco, int env) {
    LifeQualityScore += lifeQ;
    EconomyScore += eco;
    EnvironmentScore += env;
}


//ECONOMY-SELECTION
EconomySelection::EconomySelection() : lastSelectedIndex(-1) {}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::runtime_error("No facilities available");
    }
    int size = static_cast<int>(facilitiesOptions.size());
    for (int i = 0; i < size; ++i) {
        int index = (lastSelectedIndex + 1 + i) % size;
        if (facilitiesOptions[index].getCategory() == FacilityCategory::ECONOMY) {
            lastSelectedIndex = index;
            return facilitiesOptions[index];
        }
    }
    throw std::runtime_error("No facility found"); 
}

const string EconomySelection::toString() const {
    return "EconomySelection";
}

EconomySelection* EconomySelection::clone() const {
    return new EconomySelection(*this);
}

//SUSTAINABILITY-SELECTION
SustainabilitySelection::SustainabilitySelection() : lastSelectedIndex(-1) {}

const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    if (facilitiesOptions.empty()) {
        throw std::runtime_error("No facilities available");
    }

    int size = static_cast<int>(facilitiesOptions.size());
    for (int i = 0; i < size; ++i) {
        int index = (lastSelectedIndex + 1 + i) % size;
        if (facilitiesOptions[index].getCategory() == FacilityCategory::ENVIRONMENT) {
            lastSelectedIndex = index;
            return facilitiesOptions[index];
        }
    }
    throw std::runtime_error("No facility found");
}

const string SustainabilitySelection::toString() const {
    return "SustainabilitySelection";
}

SustainabilitySelection* SustainabilitySelection::clone() const {
    return new SustainabilitySelection(*this);
}
