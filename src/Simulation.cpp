#pragma once
#include "Simulation.h"
#include <iostream>
#include <algorithm>
using namespace std;

//Constructo
//TO DO:
Simulation::Simulation(const string &configFilePath)
    : isRunning(false), type(type) {}


const void Simulation::open(){
    isRunning = true;
}
