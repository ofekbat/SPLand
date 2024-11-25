#include "Action.h"
#include "Simulation.h"
#include <iostream>

using std::cout;
using std::endl;

// BaseAction Implementation
BaseAction::BaseAction() : errorMsg(""), status(ActionStatus::COMPLETED) {}

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
    for (int i = 0; i < numOfSteps; ++i) {
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const {
    return "SimulateStep " + std::to_string(numOfSteps);
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