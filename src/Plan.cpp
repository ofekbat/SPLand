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


void Plan::step() {
    
    for (auto& facility : underConstruction) {
        FacilityStatus currentStatus = facility->step();

        if (currentStatus == FacilityStatus::OPERATIONAL) {
            facilities.push_back(std::move(facility));
            life_quality_score += facility->getLifeQualityScore();
            economy_score += facility->getEconomyScore();
            environment_score += facility->getEnvironmentScore();
        }
    }
    for (auto it = underConstruction.begin(); it != underConstruction.end();) {
        if ((*it)->getStatus() == FacilityStatus::OPERATIONAL) {
            it = underConstruction.erase(it);
        } else {
            ++it;
        }
    }
    if (underConstruction.empty() && !facilities.empty()) {
        status = PlanStatus::AVALIABLE;
    } else {
        status = PlanStatus::BUSY;
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
}
    
void Plan::addFacility(Facility* facility) {
    underConstruction.push_back(facility);
}

const string Plan::toString() const {
   return "Plan ID: " + to_string(plan_id) + "\n" +
       "Settlement Name: " + settlement.getName() + "\n" +
       "Plan Status: " + (status == PlanStatus::AVALIABLE ? "Available" : "BUSY");

}