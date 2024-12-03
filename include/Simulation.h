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
        //constructor
        Simulation(const string &configFilePath);

        //rule of 5
        ~Simulation();
        Simulation(const Simulation &other);
        Simulation& operator=(const Simulation &other);

        //methods
        void start();
        void step();
        void close();
        void open();
        void backUp();
        bool restore();

        //add
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);

        //check
        bool isSettlementExists(const string &settlementName);
        bool isPlanExists(int plan_id); //check if exist

        //getters
        Settlement &getSettlement(const string &settlementName);
        const vector<BaseAction*>& getActionsLog() const; //getter for the actions
        Plan &getPlan(const int planID);
        


    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;
};

    extern Simulation* backup; 
