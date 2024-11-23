#include "Simulation.h"
#include "Auxiliary.h"
#include "Settlement.h"
#include "Plan.h"
#include "Facility.h"
#include "SelectionPolicy.h"
#include <fstream>

Simulation::Simulation(const std::string &configFilePath) : isRunning(false), planCounter(0) {
    std::ifstream configFile(configFilePath);

    std::string line;
    while (std::getline(configFile, line)) {
        std::vector<std::string> arguments = Auxiliary::parseArguments(line);

        if (arguments[0] == "settlement") {
            Settlement settlement(arguments[1], static_cast<SettlementType>(std::stoi(arguments[2])));
            settlements.push_back(settlement);
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