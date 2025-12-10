#pragma once

#include "System/System.hpp"
#include "Integrator/Integrator.hpp"
#include "OutputManager/OutputManager.hpp"
#include <iostream>
#include <Eigen/Dense>
#include <vector>

class DynamicEngine {
private:
    System Systeme;
    Integrator& Scheme;
    ForceCalculator& ForceCal;
    OutputManager& OutputMgr;
    double dt;
public:
    // ----------------------------------------------
    // Constructor
    // ----------------------------------------------
    DynamicEngine(System Systeme,
                    Integrator& Scheme,
                    ForceCalculator& ForceCal,
		    OutputManager& OutputMgr,
                    double dt);
    // ----------------------------------------------
    // geter
    // ----------------------------------------------
    const System& getSystem() const { return Systeme; }
    ForceCalculator& getForceCalculator() {return ForceCal;}
    Integrator& getIntegrator() {return Scheme;}
    // ----------------------------------------------
    // Methods
    // ----------------------------------------------
    void run(double T);
};
