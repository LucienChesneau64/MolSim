#pragma once

#include "Integrator/Integrator.hpp"
#include "System/System.hpp"
#include <Eigen/Dense>
#include <iostream>


class Verlet : public Integrator {
    public:
    // ----------------------------------	
    // Constructeur 
    // ----------------------------------
    Verlet(ForceCalculator& fc) : Integrator(fc) {}

    // ----------------------------------
    // Methods
    // ----------------------------------
    void Step(System& system, double dt) override;
};
