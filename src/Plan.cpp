#include "Plan.h"
#include <iostream>
#include <algorithm>
using namespace std;

// Constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), facilityOptions(facilityOptions),
      life_quality_score(0), economy_score(0), environment_score(0), status(PlanStatus::AVALIABLE) {}

//Destructor
Plan::~Plan() {
    for (auto facility : underConstruction) {
        delete facility;
    }
    for (auto facility : facilities) {
        delete facility;
    }
    delete selectionPolicy;
}

//Copy constructor
Plan::Plan(const Plan& other)
    : plan_id(other.plan_id), settlement(other.settlement), selectionPolicy(other.selectionPolicy->clone()),
    facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score), economy_score(other.economy_score),
    environment_score(other.environment_score), status(other.status) {
    
    // Copy underConstruction
    for (const auto& facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }

    // Copy facilities
    for (const auto& facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }
}

//Copy Assignment Operator
Plan& Plan::operator=(const Plan& other) {
    if (this == &other) return *this;

    // Clean up existing resources
    for (auto facility : underConstruction) delete facility;
    underConstruction.clear();
    for (auto facility : facilities) delete facility;
    facilities.clear();
    delete selectionPolicy;

    // Copy fields
    plan_id = other.plan_id;
    settlement = other.settlement;
    selectionPolicy = other.selectionPolicy->clone();
    facilityOptions = other.facilityOptions;
    life_quality_score = other.life_quality_score;
    economy_score = other.economy_score;
    environment_score = other.environment_score;
    status = other.status;

    // Copy underConstruction facilities
    for (const auto& facility : other.underConstruction) {
        underConstruction.push_back(new Facility(*facility));
    }

    // Copy operational facilities
    for (const auto& facility : other.facilities) {
        facilities.push_back(new Facility(*facility));
    }

    return *this;
}

//Move Constructor
Plan::Plan(Plan&& other) noexcept
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(other.selectionPolicy),
      facilityOptions(std::move(other.facilityOptions)),
      underConstruction(std::move(other.underConstruction)),
      facilities(std::move(other.facilities)),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score),
      status(other.status) {

    // Nullify pointers and clear original object's resources
    other.selectionPolicy = nullptr;
    other.underConstruction.clear();
    other.facilities.clear();
}

//Move Assignment Operator
Plan& Plan::operator=(Plan&& other) noexcept {
    if (this == &other) return *this; // הגנה מפני השמה עצמית

    // Clean up existing resources
    for (auto facility : underConstruction) delete facility;
    underConstruction.clear();
    for (auto facility : facilities) delete facility;
    facilities.clear();
    delete selectionPolicy;

    // Move fields
    plan_id = other.plan_id;
    settlement = other.settlement;
    selectionPolicy = other.selectionPolicy;
    facilityOptions = std::move(other.facilityOptions);
    underConstruction = std::move(other.underConstruction);
    facilities = std::move(other.facilities);
    life_quality_score = other.life_quality_score;
    economy_score = other.economy_score;
    environment_score = other.environment_score;
    status = other.status;

    // Nullify pointers and clear original object's resources
    other.selectionPolicy = nullptr;
    other.underConstruction.clear();
    other.facilities.clear();

    return *this;
}





//Getters
const int Plan::getlifeQualityScore() const{
    return life_quality_score;
}

const int Plan::getEconomyScore() const{
    return economy_score;
}

const int Plan::getEnvironmentScore() const{
    return environment_score;
}

const vector<Facility*>& Plan::getFacilities() const{
    return facilities;
}

//Update the selection policy
void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy) {
    delete this->selectionPolicy;
    selectionPolicy = newPolicy->clone();
}


//TO DO:
//Perform a simulation step
void Plan::step() {
    for (auto& facility : underConstruction) {
        if (facility->step() == FacilityStatus::OPERATIONAL) {
            facilities.push_back(facility);
        }
    }
    underConstruction.erase(remove_if(underConstruction.begin(), underConstruction.end(),
        [](Facility* facility) { return facility->getStatus() == FacilityStatus::OPERATIONAL; }),
        underConstruction.end());
}

//Print the current status of the plan
void Plan::printStatus() {
    cout << toString() << endl;
    cout << "Facilities under construction: " << underConstruction.size() << endl;
    cout << "Operational facilities: " << facilities.size() << endl;
    cout << "Life Quality Score: " << life_quality_score << endl;
    cout << "Economy Score: " << economy_score << endl;
    cout << "Environment Score: " << environment_score << endl;
}

//Add a facility to the plan
void Plan::addFacility(Facility* facility) {
    underConstruction.push_back(facility);
}



//Convert the plan to a string
const string Plan::toString() const {
    return "Plan ID: " + to_string(plan_id) +
           ", Settlement: " + settlement.getName() +
           ", Status: " + (status == PlanStatus::AVALIABLE ? "Available" : "Completed");
}


