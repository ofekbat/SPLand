#include "Simulation.h"
#include "Auxiliary.h"
#include <fstream>
#include <iostream>

using std::cout;
using std::endl;

Simulation::Simulation(const std::string &configFilePath) : isRunning(false), planCounter(0) {
    std::ifstream configFile(configFilePath);
    std::string line;
    while (std::getline(configFile, line)) {
        std::vector<std::string> arguments = Auxiliary::parseArguments(line);

        if (arguments[0] == "settlement") {
            Settlement settlement(arguments[1], static_cast<SettlementType>(std::stoi(arguments[2])));
            settlements.push_back(&settlement);
        } else if (arguments[0] == "facility") {
            FacilityType facility(arguments[1],
                                  static_cast<FacilityCategory>(std::stoi(arguments[2])),
                                  std::stoi(arguments[3]),
                                  std::stoi(arguments[4]),
                                  std::stoi(arguments[5]),
                                  std::stoi(arguments[6]));
            facilitiesOptions.push_back(facility);
        } else if (arguments[0] == "plan") {
            Settlement &settlement = getSettlement(arguments[1]);
            SelectionPolicy *policy = nullptr;

            if (arguments[2] == "nve") {
                policy = new NaiveSelection();
            } else if (arguments[2] == "bal") {
                policy = new BalancedSelection(0, 0, 0);
            } else if (arguments[2] == "eco") {
                policy = new EconomySelection();
            } else if (arguments[2] == "env") {
                policy = new SustainabilitySelection();
            }

            Plan plan(planCounter++, settlement, policy, facilitiesOptions);
            plans.push_back(plan);
        }
    }
}

void Simulation::start() {
    cout << "The simulation has started" << endl;
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy) {
    plans.emplace_back(planCounter++, settlement, selectionPolicy, facilitiesOptions);
}

void Simulation::addAction(BaseAction *action) {
    actionsLog.push_back(master_ptr<BaseAction>(action));
}

bool Simulation::addSettlement(Settlement *settlement) {
    for (const auto &s : settlements) {
        if (s->getName() == settlement->getName()) {
            return false;
        }
    }
    settlements.push_back(settlement);
    return true;
}

bool Simulation::addFacility(FacilityType facility) {
    for (const auto &f : facilitiesOptions) {
        if (f.getName() == facility.getName()) {
            return false;
        }
    }
    facilitiesOptions.push_back(facility);
    return true;
}

bool Simulation::isSettlementExists(const string &settlementName) {
    for (const auto &settlement : settlements) {
        if (settlement->getName() == settlementName) {
            return true;
        }
    }
    return false;
}

Settlement &Simulation::getSettlement(const string &settlementName) {
    for (auto &settlement : settlements) {
        if (settlement->getName() == settlementName) {
            return *settlement;
        }
    }
    throw std::runtime_error("Settlement not found");
}

Plan &Simulation::getPlan(const int planID) {
    for (auto &plan : plans) {
        if (planID == plan.getPlanId()) { // Use the getter here
            return plan;
        }
    }
    throw std::runtime_error("Plan not found");
}

void Simulation::step() {
    for (auto &plan : plans) {
        plan.step();
    }
}

void Simulation::close() {
    isRunning = false;
    settlements.clear();
    actionsLog.clear();
    cout << "Simulation closed" << endl;
}

