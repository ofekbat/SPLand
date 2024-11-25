#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
#include "master_ptr.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        //new getter
        int getPlanId() const;
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;

    private:
        int plan_id;
        const Settlement &settlement;
        //changed
        master_ptr<SelectionPolicy> selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        
        //changed - 2
        vector<master_ptr<Facility>> facilities;
        vector<master_ptr<Facility>> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
        //add beacuse the use of master_ptr
        mutable vector<Facility*> cachedFacilities;
};