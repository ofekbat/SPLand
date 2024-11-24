#include "Action.h"

//abstract class - Base action
BaseAction::BaseAction() : status(ActionStatus::COMPLETED), errorMsg("") {}

//getter 
ActionStatus BaseAction::getStatus() const {
    return status;
}

const std::string& BaseAction::getErrorMsg() const {
    return errorMsg;
}

//change status to complete
void BaseAction::complete() {
    status = ActionStatus::COMPLETED;
}

//complete cannot process - return error messege
void BaseAction::error(std::string errorMsg) {
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
}

//SimulateStep class
SimulateStep::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(Simulation &simulation) {
    for (int i = 0; i < numOfSteps; ++i) {
        simulation.step();
    }
    complete();
}

const std::string SimulateStep::toString() const {
    return "SimulateStep " + std::to_string(numOfSteps);
}

SimulateStep* SimulateStep::clone() const {
    return new SimulateStep(*this);
}

//AddPlan class
AddPlan::AddPlan(const std::string &settlementName, const std::string &selectionPolicy)
    : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

void AddPlan::act(Simulation &simulation) {
    if (!simulation.addPlan(settlementName, selectionPolicy)) {
        error("Cannot create this plan");
    } else {
        complete();
    }
}

const std::string AddPlan::toString() const {
    return "AddPlan " + settlementName + " " + selectionPolicy;
}

AddPlan* AddPlan::clone() const {
    return new AddPlan(*this);
}

//AddSettlement class
AddSettlement::AddSettlement(const std::string &settlementName, SettlementType settlementType)
    : settlementName(settlementName), settlementType(settlementType) {}

void AddSettlement::act(Simulation &simulation) {
    if (!simulation.addSettlement(settlementName, settlementType)) {
        error("Settlement already exists");
    } else {
        complete();
    }
}

const std::string AddSettlement::toString() const {
    return "AddSettlement " + settlementName;
}

AddSettlement* AddSettlement::clone() const {
    return new AddSettlement(*this);
}

//AddFacility class
AddFacility::AddFacility(const std::string &facilityName, FacilityCategory facilityCategory, int price,
                         int lifeQualityScore, int economyScore, int environmentScore)
    : facilityName(facilityName), facilityCategory(facilityCategory), price(price),
      lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}

void AddFacility::act(Simulation &simulation) {
    if (!simulation.addFacility(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore)) {
        error("Facility already exists");
    } else {
        complete();
    }
}

const std::string AddFacility::toString() const {
    return "AddFacility " + facilityName;
}

AddFacility* AddFacility::clone() const {
    return new AddFacility(*this);
}

//PrintPlanStatus class
PrintPlanStatus::PrintPlanStatus(int planId) : planId(planId) {}

void PrintPlanStatus::act(Simulation &simulation) {
    if (!simulation.printPlanStatus(planId)) {
        error("Plan doesn’t exist");
    } else {
        complete();
    }
}

const std::string PrintPlanStatus::toString() const {
    return "PrintPlanStatus " + std::to_string(planId);
}

PrintPlanStatus* PrintPlanStatus::clone() const {
    return new PrintPlanStatus(*this);
}


//ChangePlanPolicy class
ChangePlanPolicy::ChangePlanPolicy(int planId, const std::string &newPolicy)
    : planId(planId), newPolicy(newPolicy) {}

void ChangePlanPolicy::act(Simulation &simulation) {
    if (!simulation.changePlanPolicy(planId, newPolicy)) {
        error("Cannot change selection policy");
    } else {
        complete();
    }
}

const std::string ChangePlanPolicy::toString() const {
    return "ChangePlanPolicy " + std::to_string(planId) + " " + newPolicy;
}

ChangePlanPolicy* ChangePlanPolicy::clone() const {
    return new ChangePlanPolicy(*this);
}


//PrintActionsLog class
PrintActionsLog::PrintActionsLog() {}

void PrintActionsLog::act(Simulation &simulation) {
    simulation.printActionsLog();
    complete();
}

const std::string PrintActionsLog::toString() const {
    return "PrintActionsLog";
}

PrintActionsLog* PrintActionsLog::clone() const {
    return new PrintActionsLog(*this);
}


//Close class
Close::Close() {}

void Close::act(Simulation &simulation) {
    simulation.close();
    complete();
}

const std::string Close::toString() const {
    return "Close";
}

Close* Close::clone() const {
    return new Close(*this);
}


//BackupSimulation class
BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation &simulation) {
    simulation.backup();
    complete();
}

const std::string BackupSimulation::toString() const {
    return "BackupSimulation";
}

BackupSimulation* BackupSimulation::clone() const {
    return new BackupSimulation(*this);
}


//RestoreSimulation class
RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation &simulation) {
    if (!simulation.restore()) {
        error("No backup available");
    } else {
        complete();
    }
}

const std::string RestoreSimulation::toString() const {
    return "RestoreSimulation";
}

RestoreSimulation* RestoreSimulation::clone() const {
    return new RestoreSimulation(*this);
}
