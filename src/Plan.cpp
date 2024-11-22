// #include "Plan.h"
// #include <iostream>
// #include <algorithm>
// using namespace std;

// // Constructor
// Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
//     : plan_id(planId), settlement(settlement), selectionPolicy(selectionPolicy), facilityOptions(facilityOptions),
//       life_quality_score(0), economy_score(0), environment_score(0), status(PlanStatus::AVALIABLE) {}

// //Destructor
// //check if needed
// Plan::~Plan() {
//     for (auto facility : underConstruction) {
//         delete facility;
//     }
//     for (auto facility : facilities) {
//         delete facility;
//     }
//     delete selectionPolicy;
// }

// //Getters
// const int Plan::getlifeQualityScore() const{
//     return life_quality_score;
// }

// const int Plan::getEconomyScore() const{
//     return economy_score;
// }

// const int Plan::getEnvironmentScore() const{
//     return environment_score;
// }

// const vector<Facility*>& Plan::getFacilities() const{
//     return facilities;
// }

// //Update the selection policy
// void Plan::setSelectionPolicy(SelectionPolicy *newPolicy) {
//     delete selectionPolicy;
//     selectionPolicy = newPolicy->clone();
// }


// //TO DO:
// //Perform a simulation step
// void Plan::step() {
//     for (auto& facility : underConstruction) {
//         if (facility->step() == FacilityStatus::OPERATIONAL) {
//             facilities.push_back(facility);
//         }
//     }
//     underConstruction.erase(remove_if(underConstruction.begin(), underConstruction.end(),
//         [](Facility* facility) { return facility->getStatus() == FacilityStatus::OPERATIONAL; }),
//         underConstruction.end());
// }

// //Print the current status of the plan
// void Plan::printStatus() {
//     cout << toString() << endl;
//     cout << "Facilities under construction: " << underConstruction.size() << endl;
//     cout << "Operational facilities: " << facilities.size() << endl;
//     cout << "Life Quality Score: " << life_quality_score << endl;
//     cout << "Economy Score: " << economy_score << endl;
//     cout << "Environment Score: " << environment_score << endl;
// }

// //Add a facility to the plan
// void Plan::addFacility(Facility* facility) {
//     underConstruction.push_back(facility);
// }



// //Convert the plan to a string
// const string Plan::toString() const {
//     return "Plan ID: " + to_string(plan_id) +
//            ", Settlement: " + settlement.getName() +
//            ", Status: " + (status == PlanStatus::AVALIABLE ? "Available" : "Completed");
// }


