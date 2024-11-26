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

    backup = nullptr;
}

void Simulation::start() {
    cout << "The simulation has started" << endl;
    std::vector<std::string> args;
    isRunning = true;
    std::string command;
    while (isRunning) {
        std::getline(std::cin, command);
        
        args = Auxiliary::parseArguments(command);
        if (args.empty()) {
            std::cout << "Error: No action provided" << std::endl;
            return;
        }
        try {
            const std::string& actionType = args[0];
            BaseAction* action = nullptr; 

        
            if (actionType == "step") {
                if (args.size() != 2) {
                    std::cout << "Error: Invalid arguments for step action" << std::endl;
                    return;
                }
                int steps = std::stoi(args[1]);
                action = new SimulateStep(steps);
                action->act(*this);
                addAction(action);

            } else if (actionType == "plan") {
                if (args.size() != 3) {
                    std::cout << "Error: Invalid arguments for plan action" << std::endl;
                    return;
                }
                action = new AddPlan(args[1], args[2]);
                action->act(*this);
                addAction(action);

            } else if (actionType == "facility") {
                if (args.size() != 6) {
                    std::cout << "Error: Invalid arguments for facility action" << std::endl;
                    return;
                }
                FacilityCategory category = static_cast<FacilityCategory>(std::stoi(args[2]));
                int price = std::stoi(args[3]);
                int lifeQualityScore = std::stoi(args[4]);
                int economyScore = std::stoi(args[5]);
                int environmentScore = std::stoi(args[6]);
                action = new AddFacility(args[1], category, price, lifeQualityScore, economyScore, environmentScore);
                action->act(*this);
                addAction(action);
            } else if (actionType == "settlement") {
                if (args.size() != 3)
                {
                    std::cout << "Error: Invalid arguments for settlement action" << std::endl;
                    return;
                }
                SettlementType settlementType = static_cast<SettlementType>(std::stoi(args[2]));
                action = new AddSettlement(args[1], settlementType);
                action->act(*this);
                addAction(action);
                } else if (actionType == "planStatus") { 
                    if (args.size() != 2) {
                        std::cout << "Error: Invalid arguments for planStatus action" << std::endl;
                        return;
                    }
                int planID = std::stoi(args[1]);
                Plan& plan = getPlan(planID);
                plan.printStatus();
            } else if (actionType == "backup") {
                backUp();
                std::cout << "Simulation state has been backed up." << std::endl;

            } else if (actionType == "restore") {
                if (restore()) {
                    std::cout << "Simulation state has been restored from backup." << std::endl;
                } else {
                    std::cout << "Error: No backup available to restore." << std::endl;
                }

            } else if (actionType == "log") {
                for (const auto& action : actionsLog) {
                    std::cout << action.get()->toString() << std::endl;
                }
                
            } else if (actionType == "close") {
                action = new Close();
                action->act(*this);
                addAction(action);
                isRunning = false;
            } else {
                std::cout << "Error: Unknown action \"" << actionType << "\"" << std::endl;
            }

            if (action) {
                action->act(*this); 
                addAction(action); 
            }
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
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

void Simulation::backUp() {
    if (backup) delete backup; 
    backup = new Simulation(*this); 
}

 bool Simulation::restore() {
//     if (!backup) return false;
//     *this = *backup; 
        return true;
 }

