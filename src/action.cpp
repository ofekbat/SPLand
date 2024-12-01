
#include "Action.h"
#include "Simulation.h"
#include <iostream>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;
using std::string;

// BaseAction Implementation
BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::ERROR) {}

ActionStatus BaseAction::getStatus() const {
    return status;
}

void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

void BaseAction::error(string errorMsg) {
    this->errorMsg = errorMsg;
    status = ActionStatus::ERROR;
    cout << "Error: " << errorMsg << endl;
}

const string &BaseAction::getErrorMsg() const {
    return errorMsg;
}

// SimulateStep Implementation
SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation) {
    for (int i = 0; i < numOfSteps; i++) {
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const {
    return "Step " + std::to_string(numOfSteps);
}

SimulateStep *SimulateStep::clone() const {
    return new SimulateStep(*this);
}

// AddPlan Implementation
AddPlan::AddPlan(const string &settlementName, const string &selectionPolicy)
    : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation) {
    if (!simulation.isSettlementExists(settlementName)) {
        error("Cannot create this plan");
        return;
    }

    SelectionPolicy *policy = nullptr;

    if (selectionPolicy == "nve")
        policy = new NaiveSelection();
    else if (selectionPolicy == "bal")
        policy = new BalancedSelection(0, 0, 0);
    else if (selectionPolicy == "eco")
        policy = new EconomySelection();
    else if (selectionPolicy == "env")
        policy = new SustainabilitySelection();
    else {
        error("Cannot create this plan");
        return;
    }

    simulation.addPlan(simulation.getSettlement(settlementName), policy);
    complete();
}

const string AddPlan::toString() const {
    return "AddPlan " + settlementName + " " + selectionPolicy;
}

AddPlan *AddPlan::clone() const {
    return new AddPlan(*this);
}


//AddSettlement class
AddSettlement::AddSettlement(const string &settlementName, SettlementType settlementType)
    : settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation) {
    if (simulation.isSettlementExists(settlementName)) {
        error("Settlement already exists");
        return;
    }
    Settlement* newSettlement = new Settlement(settlementName, settlementType);
    if (!simulation.addSettlement(newSettlement)) {
        delete newSettlement; // ניקוי זיכרון אם הפעולה נכשלה
        error("Failed to add settlement");
        return;
    }
    complete();
}

const string AddSettlement::toString() const {
    return "AddSettlement " + settlementName + " " + std::to_string(static_cast<int>(settlementType));
}

AddSettlement *AddSettlement::clone() const {
    return new AddSettlement(*this);
}

//AddFacility class
AddFacility::AddFacility(const string &facilityName, const FacilityCategory facilityCategory,
                         const int price, const int lifeQualityScore, const int economyScore,
                         const int environmentScore)
    : facilityName(facilityName), facilityCategory(facilityCategory), price(price),
      lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation) {
    FacilityType newFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore);
    if (!simulation.addFacility(newFacility)) {
        error("Facility already exists");
        return;
    }
    complete();
}

const string AddFacility::toString() const {
    return "AddFacility " + facilityName + " " + std::to_string(static_cast<int>(facilityCategory)) +
           " " + std::to_string(price) + " " + std::to_string(lifeQualityScore) +
           " " + std::to_string(economyScore) + " " + std::to_string(environmentScore);
}

AddFacility *AddFacility::clone() const {
    return new AddFacility(*this);
}


//PrintPlanStatus class
PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation) {
    if (!simulation.isPlanExists(planId)) {
        error("Plan doesn’t exist");
        return;
    }
    simulation.getPlan(planId).printStatus();
    complete();
}

const string PrintPlanStatus::toString() const {
    return "PlanStatus " + std::to_string(planId);
}

PrintPlanStatus *PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}


//ChangePlanPolicy class
ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy)
    : planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation) {
    
    if (!simulation.isPlanExists(planId)) {
        error("Cannot change selection policy");
        return;
    }
    Plan& plan = simulation.getPlan(planId);
    if (newPolicy == "bal") {
        // Retrieve current construction scores from the plan
        int lifeQScore = plan.getlifeQualityScore();
        int ecoScore = plan.getEconomyScore();
        int envScore = plan.getEnvironmentScore();

        int LifeQualityC = 0;
        int EconomyC = 0;
        int EnvironmentC = 0;
        for (const Facility* facility : plan.getUnderConstruction()) {
            LifeQualityC += facility->getLifeQualityScore();
            EconomyC += facility->getEconomyScore();
            EnvironmentC += facility->getEnvironmentScore();
        }

        // Create BalancedSelection with current scores
        auto* balancedPolicy = new BalancedSelection(lifeQScore, ecoScore, envScore);
        balancedPolicy->updateUnderConstruction(LifeQualityC, EconomyC, EnvironmentC);

        // Update the plan's policy
        plan.setSelectionPolicy(balancedPolicy);

    } else if (newPolicy == "nve") {
        plan.setSelectionPolicy(new NaiveSelection());
    } else if (newPolicy == "eco") {
        plan.setSelectionPolicy(new EconomySelection());
    } else if (newPolicy == "env") {
        plan.setSelectionPolicy(new SustainabilitySelection());
    } else {
        error("Invalid selection policy type: " + newPolicy);
    }
    
    complete();
}

const string ChangePlanPolicy::toString() const {
    return "ChangePlanPolicy " + std::to_string(planId) + " " + newPolicy;
}

ChangePlanPolicy *ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}


//Close class
Close::Close() {}

void Close::act(Simulation &simulation) {
    simulation.close();
    complete();
}

const string Close::toString() const {
    return "Close";
}

Close *Close::clone() const {
    return new Close(*this);
}


//PrintActionsLog class
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation &simulation) {
    for (const auto &action : simulation.getActionsLog()) {
        cout << action->toString() << " " 
             << (action->getStatus() == ActionStatus::COMPLETED ? "COMPLETED" : "ERROR") << endl;
    }
    complete();
}

const string PrintActionsLog::toString() const {
    return "PrintActionsLog";
}

PrintActionsLog *PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}


//BackupSimulation class
BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation &simulation) {
    simulation.backUp();
    complete();
}

const string BackupSimulation::toString() const {
    return "Backup";
}

BackupSimulation *BackupSimulation::clone() const {
    return new BackupSimulation(*this);
}


//RestoreSimulation class
RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation &simulation) {
    if (simulation.restore()) {
        complete();
    } else {
        error("Restore failed: No backup available.");
    }
}

const string RestoreSimulation::toString() const {
    return "RestoreSimulation";
}

RestoreSimulation *RestoreSimulation::clone() const {
    return new RestoreSimulation(*this);
}
