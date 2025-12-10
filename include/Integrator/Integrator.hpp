#pragma once

#include "ForceCalculator/ForceCalculator.hpp"
#include "System/System.hpp"
#include <iostream>

class Integrator {
protected:
    ForceCalculator& force_calculator; 

public:
    // Constructeur obligatoire pour initialiser la référence
    Integrator(ForceCalculator& fc) : force_calculator(fc) {}



    // destructeur
    virtual ~Integrator() = default;

    virtual void Step(System& system, double dt) = 0; 
};
