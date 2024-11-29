#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Action.h"
using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        ~Simulation();
        Simulation(const Simulation &other);
        Simulation& operator=(const Simulation &other);
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        bool isPlanExists(int plan_id); //check if exist
        Settlement &getSettlement(const string &settlementName);
        const vector<BaseAction*>& getActionsLog() const; //getter for the actions
        Plan &getPlan(const int planID);
        void step();
        void close();
        void open();
        void backUp();
        bool restore();


    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
};

    extern Simulation* backup; 
