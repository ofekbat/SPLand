#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        //constructor
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);

        //rule of 5
         ~Plan();
        Plan(const Plan &other);
        Plan &operator=(const Plan &other); 

        //getters
        const std::vector<Facility*>& getUnderConstruction() const;
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        const PlanStatus getStatus() const;
        const Settlement getSettlement() const;
        int getPlanId() const;
        const vector<Facility*> &getFacilities() const;

        //methods
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        void addFacility(Facility* facility);
        void update_score(int life, int economy, int envirmont);

        //print
        void printStatus();
        const string toString() const;
        const void closetoString() const;

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;        
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};