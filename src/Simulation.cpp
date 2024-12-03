#include "Simulation.h"
#include "Auxiliary.h"
#include <fstream>
#include <iostream>

using std::cout;
using std::endl;

Simulation::Simulation(const std::string &configFilePath) : 
    isRunning(false), 
    planCounter(0), 
    actionsLog(), 
    plans(), 
    settlements(), 
    facilitiesOptions()
{
    std::ifstream configFile(configFilePath);
    std::string line;
    while (std::getline(configFile, line)) {
        std::vector<std::string> arguments = Auxiliary::parseArguments(line);
        if (arguments[0] == "settlement") {
            settlements.push_back(new Settlement(arguments[1], static_cast<SettlementType>(std::stoi(arguments[2])))); 
        } else if (arguments[0] == "facility") {
            FacilityType facility(arguments[1],
                                  static_cast<FacilityCategory>(std::stoi(arguments[2])),
                                  std::stoi(arguments[3]),
                                  std::stoi(arguments[4]),
                                  std::stoi(arguments[5]),
                                  std::stoi(arguments[6]));
            facilitiesOptions.push_back(std::move(facility));

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
            Plan plan(planCounter, settlement, policy, facilitiesOptions);
            planCounter++;
            plans.push_back(plan);
        }
    }
    backup = nullptr;
}

Simulation::~Simulation() {
    for (BaseAction* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();

    for (Settlement* settlement : settlements) {
        delete settlement;
    }
    settlements.clear();
}

Simulation::Simulation(const Simulation &other)
    : isRunning(other.isRunning),
      planCounter(other.planCounter),
      actionsLog(),
      plans(other.plans),
      settlements(),
      facilitiesOptions(other.facilitiesOptions)
      
{
    for (BaseAction* action : other.actionsLog) {
        actionsLog.push_back(action->clone());  //using BaseAction clone() method
    }

    for (Settlement* settlement : other.settlements) {
        settlements.push_back(new Settlement(*settlement)); //using Settlement has a copy constructor
    }
}

Simulation& Simulation::operator=(const Simulation &other) {
    if (this != &other) {
        // Free existing resources
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog.clear();

        for (Settlement* settlement : settlements) {
            delete settlement;
        }
        settlements.clear();

        isRunning = other.isRunning;
        planCounter = other.planCounter;

        plans = other.plans;
        facilitiesOptions = other.facilitiesOptions;

        for (BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone()); //using BaseAction clone() method
        }

        for (Settlement* settlement : other.settlements) {
            settlements.push_back(new Settlement(*settlement)); //using Settlement has a copy constructor
        }
    }
    return *this;
}

void Simulation::start() {
    std::vector<std::string> args;
    open();
    std::string command;
    std::cout << "The simulation has started" << std::endl;
    while (isRunning) {
        std::getline(std::cin, command);
        
        args = Auxiliary::parseArguments(command);
        if (args.empty()) {
            std::cout << "Error: No action provided" << std::endl;
            return;
        }
            const std::string& actionType = args[0];
            BaseAction* action = nullptr; 
            if (actionType == "step") {
                int steps = std::stoi(args[1]);
                action = new SimulateStep(steps);

            } else if (actionType == "plan") {
                action = new AddPlan(args[1], args[2]);

            } else if (actionType == "facility") {
                FacilityCategory category = static_cast<FacilityCategory>(std::stoi(args[2]));
                int price = std::stoi(args[3]);
                int lifeQualityScore = std::stoi(args[4]);
                int economyScore = std::stoi(args[5]);
                int environmentScore = std::stoi(args[6]);
                action = new AddFacility(args[1], category, price, lifeQualityScore, economyScore, environmentScore);
            } else if (actionType == "settlement") {
                SettlementType settlementType = static_cast<SettlementType>(std::stoi(args[2]));
                action = new AddSettlement(args[1], settlementType);
                } else if (actionType == "planStatus") { 
                int planID = std::stoi(args[1]);
                action = new PrintPlanStatus(planID);
            } else if (actionType == "backup") {
                action = new BackupSimulation();
            } else if (actionType == "restore") {
                action = new RestoreSimulation();
            } else if (actionType == "log") {
                action = new PrintActionsLog();
            } else if(actionType == "changePolicy"){
                action = new ChangePlanPolicy(std::stoi(args[1]), args[2]);
            }
            else if (actionType == "close") {
                action = new Close();
                action->act(*this);
                delete action;
            }
            if (action && dynamic_cast<Close*>(action) == nullptr) {
                    action->act(*this);
                    addAction(action);
            }
            if(action == nullptr){
                std::cout << "Error: Unknown action \"" << actionType << "\"" << std::endl;
            }
    }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy) {
    plans.emplace_back(planCounter++, settlement, selectionPolicy, facilitiesOptions);
}

void Simulation::addAction(BaseAction *action) {
    actionsLog.push_back(action);
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
    facilitiesOptions.push_back(std::move(facility));
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
    try {
        for (auto &settlement : settlements) {
            if (settlement->getName() == settlementName) {
                return *settlement;
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
    throw std::runtime_error("Settlement not found: " + settlementName);
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

void Simulation::backUp() {
    if (backup) delete backup; 
    backup = new Simulation(*this); 
}

bool Simulation::restore() {
    if (!backup) {
        std::cerr << "No backup available" << std::endl;
        return false;
    }

    for (BaseAction* action : actionsLog) {
        delete action;
    }
    actionsLog.clear();

    for (Settlement* settlement : settlements) {
        delete settlement;
    }
    settlements.clear();

    *this = *backup;
    return true;
}

bool Simulation::isPlanExists(int plan_id) {
    for (const auto& plan : plans) {
        if (plan.getPlanId() == plan_id) {
            return true;
        }
    }
    return false;
}

const vector<BaseAction*>& Simulation::getActionsLog() const {
    return actionsLog;
}

void Simulation::open(){
    this->isRunning = true;
}

void Simulation::close() {
    isRunning = false;
    for(auto &plan : plans){
        plan.closetoString();
    }
}

