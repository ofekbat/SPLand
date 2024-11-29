#include "Plan.h"
#include <iostream>
#include <algorithm>
using namespace std;

//constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy),
      status(PlanStatus::AVALIABLE), facilities(), underConstruction(), 
      facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0) {}

Plan::~Plan() {
    for (Facility* facility : facilities) {
        delete facility;
    }
    facilities.clear();

    for (Facility* facility : underConstruction) {
        delete facility;
    }
    underConstruction.clear();

    delete selectionPolicy;
}


Plan::Plan(const Plan &other) 
    : plan_id(other.plan_id), 
      settlement(other.settlement),
      selectionPolicy(nullptr), 
      status(other.status),
      facilities(other.facilities),
      underConstruction(other.underConstruction),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {
    copyFrom(other);
}

Plan &Plan::operator=(const Plan &other) {
    if (this != &other) {
        delete selectionPolicy;
        copyFrom(other);
    }
    return *this;
}

void Plan::copyFrom(const Plan &other) {
    plan_id = other.plan_id;
    status = other.status;
    facilities = other.facilities;
    underConstruction = other.underConstruction;
    life_quality_score = other.life_quality_score;
    economy_score = other.economy_score;
    environment_score = other.environment_score;

    // Deep copy the selectionPolicy
    selectionPolicy = other.selectionPolicy ? other.selectionPolicy->clone() : nullptr;
}

//getters
const int Plan::getlifeQualityScore() const {
    return life_quality_score;
}

int Plan::getPlanId() const {
    return plan_id;
}

const int Plan::getEconomyScore() const {
    return economy_score;
}

const int Plan::getEnvironmentScore() const {
    return environment_score;
}

const vector<Facility*>& Plan::getFacilities() const {
    return facilities; 
}

void Plan::setSelectionPolicy(SelectionPolicy *newPolicy) {
    selectionPolicy = newPolicy;
}

void Plan::update_score(int life, int economy, int envirmont){
    life_quality_score += life;
    economy_score += economy;
    environment_score += envirmont;
}

void Plan::step() {
    // Step 1: Update the status of facilities in underConstruction
    for (auto it = underConstruction.begin(); it != underConstruction.end();) {
        // Execute step() for each facility and check its status
        FacilityStatus currentStatus = (*it)->step();

        if (currentStatus == FacilityStatus::OPERATIONAL) {
            // Move the facility to the completed list
            facilities.push_back(std::move(*it));
                        
            update_score((*it)->getLifeQualityScore(),
                         (*it)->getEconomyScore(),
                         (*it)->getEnvironmentScore());

            it = underConstruction.erase(it);
        } else {
            ++it;
        }
    }

    // Step 2: Add new facilities to underConstruction based on construction limits
    int constructionLimit = settlement.getConstructionLimit();
    while (static_cast<int>(underConstruction.size()) < constructionLimit) {
        try {
            const FacilityType& selectedFacility = selectionPolicy->selectFacility(facilityOptions);

            Facility* newFacility = new Facility(selectedFacility, settlement.getName());
            underConstruction.push_back(newFacility);

        } catch (const std::exception& e) {
            break;
        }
    }

    // Step 3: Update the plan's status
    if (static_cast<int>(underConstruction.size()) == constructionLimit) {
        status = PlanStatus::BUSY; // Plan is busy if underConstruction is full
    } else {
        status = PlanStatus::AVALIABLE; // Plan is available otherwise
    }
}



void Plan::printStatus() {
    cout << toString() << endl;
    cout << "SelectionPolicy: " << selectionPolicy->toString() << endl;
    cout << "Life Quality Score: " << life_quality_score << endl;
    cout << "Economy Score: " << economy_score << endl;
    cout << "Environment Score: " << environment_score << endl;
        for (const auto& facilityPtr : facilities) {
            cout << facilityPtr->toString() << endl;
        }
        for (const auto& facilityPtr : underConstruction) {
            cout << facilityPtr->toString() << endl;
        }
}
    
void Plan::addFacility(Facility* facility) {
    underConstruction.push_back(facility);
    update_score(facility->getLifeQualityScore(), facility->getEconomyScore(),facility->getEnvironmentScore());
}

const string Plan::toString() const {
   return "Plan ID: " + to_string(plan_id) + "\n" +
       "Settlement Name: " + settlement.getName() + "\n" +
       "Plan Status: " + (status == PlanStatus::AVALIABLE ? "Available" : "BUSY");

}