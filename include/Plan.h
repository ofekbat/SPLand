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
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
         ~Plan(); // Destructor
        Plan(const Plan &other); // Copy constructor
        Plan &operator=(const Plan &other); // Copy assignment operator

        const std::vector<Facility*>& getUnderConstruction() const;
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        const PlanStatus getStatus() const;
        const Settlement getSettlement() const;

        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        int getPlanId() const;
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        void update_score(int life, int economy, int envirmont);

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