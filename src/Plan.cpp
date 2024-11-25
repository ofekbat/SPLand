#include "Plan.h"
#include <iostream>
#include <algorithm>
using namespace std;

//constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(settlement), selectionPolicy(master_ptr<SelectionPolicy>(selectionPolicy)),
      status(PlanStatus::AVALIABLE), facilities(), underConstruction(), 
      facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0), cachedFacilities() {}

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
    cachedFacilities.clear(); 
    for (const auto& facility : facilities) {
        cachedFacilities.push_back(facility.get()); 
    }
    return cachedFacilities; 
}

void Plan::setSelectionPolicy(SelectionPolicy *newPolicy) {
    selectionPolicy = master_ptr<SelectionPolicy>(newPolicy);
}

void Plan::step() {
    for (auto& facility : underConstruction) {
        if (facility.get()->step() == FacilityStatus::OPERATIONAL) {
            facilities.push_back(std::move(facility));
        }
    }
    underConstruction.erase(remove_if(underConstruction.begin(), underConstruction.end(),
        [](const master_ptr<Facility>& facility) { return facility.get()->getStatus() == FacilityStatus::OPERATIONAL; }),
        underConstruction.end());
}



void Plan::printStatus() {
    cout << toString() << endl;
    cout << "Facilities under construction: " << underConstruction.size() << endl;
    cout << "Operational facilities: " << facilities.size() << endl;
    cout << "Life Quality Score: " << life_quality_score << endl;
    cout << "Economy Score: " << economy_score << endl;
    cout << "Environment Score: " << environment_score << endl;
}
    
void Plan::addFacility(Facility* facility) {
    underConstruction.push_back(master_ptr<Facility>(facility));
}

const string Plan::toString() const {
    return "Plan ID: " + to_string(plan_id) +
           ", Settlement: " + settlement.getName() +
           ", Status: " + (status == PlanStatus::AVALIABLE ? "Available" : "Completed");
}