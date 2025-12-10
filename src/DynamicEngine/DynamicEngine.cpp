#include "DynamicEngine/DynamicEngine.hpp"
#include <iostream>
#include <utility>

// --------------------------------------------------
// Constructor
// --------------------------------------------------
DynamicEngine::DynamicEngine(
    System Systeme_in,
    Integrator& Scheme_in,
    ForceCalculator& ForceCal_in,
    OutputManager& OutputMgr_in,
    double dt_in)
    : Systeme(std::move(Systeme_in)), 
      Scheme(Scheme_in), 
      ForceCal(ForceCal_in), 
      OutputMgr(OutputMgr_in),
      dt(dt_in) {}
// ---------------------------------------------------
// Méthode
// ---------------------------------------------------
void DynamicEngine::run(double T) {

    double time = 0.0;
    int step = 0;
    
    ForceCal.computeAllForces(Systeme);
    
    OutputMgr.writeDatas(time, step);

    while (time < T) {
        Scheme.Step(Systeme, dt); 

        OutputMgr.writeDatas(time, step);

        time += dt;
	step++;
    }
    
}
