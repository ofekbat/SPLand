#include "Plan.h"
#include <iostream>
#include <algorithm>
using namespace std;

//constructor
Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), 
    settlement(settlement),
    planName(settlement.getName()),
    selectionPolicy(selectionPolicy),
    status(PlanStatus::AVALIABLE), 
    facilities(), 
    underConstruction(),
    facilityOptions(facilityOptions),
    life_quality_score(0), economy_score(0), environment_score(0) {}

Plan::~Plan() {
    for (Facility* facility : facilities) {
        delete facility;
    }
    facilities.clear();

    for (Facility* facility : underConstruction) {
        delete facility;
    }
    underConstruction.clear();

        if (selectionPolicy) {
        delete selectionPolicy;
        selectionPolicy = nullptr;
    }
}

Plan::Plan(const Plan &other) 
    : plan_id(other.plan_id), 
      settlement(other.settlement),
      planName(other.settlement.getName()),
      selectionPolicy(other.selectionPolicy ? other.selectionPolicy->clone() : nullptr),
      status(other.status),
      facilities(), 
      underConstruction(),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {
    for (const auto& facility : other.facilities) {
        facilities.push_back(facility ? new Facility(*facility) : nullptr);
    }
    
    for (const auto* facility : other.underConstruction) {
        underConstruction.push_back(facility ? new Facility(*facility) : nullptr);
    }
}

Plan& Plan::operator=(const Plan& other) {
    if (this != &other) {
        delete selectionPolicy;
        for (auto* facility : facilities) {
            delete facility;
        }
        facilities.clear();

        for (auto* facility : underConstruction) {
            delete facility;
        }
        underConstruction.clear();

        selectionPolicy = other.selectionPolicy ? other.selectionPolicy->clone() : nullptr;
        facilities.reserve(other.facilities.size());
        for (const auto& facility : other.facilities) {
            facilities.push_back(facility ? new Facility(*facility) : nullptr);
        }
        underConstruction.reserve(other.underConstruction.size());
        for (const auto* facility : other.underConstruction) {
            underConstruction.push_back(facility ? new Facility(*facility) : nullptr);
        }

        plan_id = other.plan_id;
        status = other.status;
        planName = other.settlement.getName();
        life_quality_score = other.life_quality_score;
        economy_score = other.economy_score;
        environment_score = other.environment_score;
    }
    return *this;
}

Plan::Plan(const Plan &other, const Settlement &newSettlement) 
    : plan_id(other.plan_id), 
      settlement(newSettlement),
      planName(newSettlement.getName()), 
      selectionPolicy(other.selectionPolicy ? other.selectionPolicy->clone() : nullptr),
      status(other.status),
      facilities(), 
      underConstruction(),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score) {

    for (const auto& facility : other.facilities) {
        facilities.push_back(facility ? new Facility(*facility) : nullptr);
    }

    for (const auto* facility : other.underConstruction) {
        underConstruction.push_back(facility ? new Facility(*facility) : nullptr);
    }
}

//getters
const std::vector<Facility*>& Plan::getUnderConstruction() const {
    return underConstruction;
}

const int Plan::getlifeQualityScore() const {
    return life_quality_score;
}

int Plan::getPlanId() const {
    return plan_id;
}

string Plan::getName() const {
    return planName;
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

const PlanStatus Plan::getStatus() const {
    return status;
}

const Settlement Plan::getSettlement() const{
    return settlement;
}

void Plan::setSelectionPolicy(SelectionPolicy *newPolicy) {
    if (newPolicy) {
        delete selectionPolicy; 
    }
    selectionPolicy = newPolicy;
}

void Plan::update_score(int life, int economy, int envirmont){
    life_quality_score += life;
    economy_score += economy;
    environment_score += envirmont;
    if (BalancedSelection* balancedSelection = dynamic_cast<BalancedSelection*>(selectionPolicy)) {
        balancedSelection->updateMainScores(life, economy, envirmont);
    }
}

void Plan::step() {

    int constructionLimit = settlement.getConstructionLimit();
    // Step 1: check if the plan is available - if yes, procces to step 2, if not, jump to step 3
    if(status == PlanStatus::AVALIABLE){

        //step 2: construct new facilities
        while (static_cast<int>(underConstruction.size()) < constructionLimit) {
            const FacilityType& selectedFacility = selectionPolicy->selectFacility(facilityOptions);

            Facility* newFacility = new Facility(selectedFacility, settlement.getName());
            underConstruction.push_back(newFacility);

            if (BalancedSelection* balancedSelection = dynamic_cast<BalancedSelection*>(selectionPolicy)) {
                balancedSelection->updateUnderConstruction(
                selectedFacility.getLifeQualityScore(),
                selectedFacility.getEconomyScore(),
                selectedFacility.getEnvironmentScore());
            }
        }
    }

    // Step 3: Update the status of the facilities under construction
    for (auto it = underConstruction.begin(); it != underConstruction.end();) {
        FacilityStatus currentStatus = (*it)->step();

        if (currentStatus == FacilityStatus::OPERATIONAL) {
            facilities.push_back(std::move(*it));
                        
            update_score((*it)->getLifeQualityScore(),
                         (*it)->getEconomyScore(),
                         (*it)->getEnvironmentScore());

            if (BalancedSelection* balancedSelection = dynamic_cast<BalancedSelection*>(selectionPolicy)) {
                balancedSelection->updateUnderConstruction(
                    -(*it)->getLifeQualityScore(),
                    -(*it)->getEconomyScore(),
                    -(*it)->getEnvironmentScore()
                );
            }
            
            it = underConstruction.erase(it);
        } else {
            ++it;
        }
    }
    
    // Step 4: Update the plan's status
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

const void Plan::closetoString() const {
    cout << "PlanID: " << plan_id << endl;
    cout << "SettlementName: " << settlement.getName() << endl;
    cout << "Life Quality Score: " << life_quality_score << endl;
    cout << "Economy Score: " << economy_score << endl;
    cout << "Environment Score: " << environment_score << endl;
}

