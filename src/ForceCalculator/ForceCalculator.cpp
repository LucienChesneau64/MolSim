#include "ForceCalculator/ForceCalculator.hpp"
#include <iostream>
#include <cmath>
#include <Eigen/Dense>


// ----------------------------------------------------
// Constructeur
// ----------------------------------------------------

ForceCalculator::ForceCalculator() {}


// ---------------
// getter
// ---------------

const std::vector<std::unique_ptr<ForceContribution>>& ForceCalculator::getActiveForces() const {return ActiveForces;}


// ----------------
// Méthode
// ----------------


void ForceCalculator::addForce(std::unique_ptr<ForceContribution> newForce) {
	ActiveForces.push_back(std::move(newForce));
}


void ForceCalculator::computeAllForces(System& system) {
    system.resetForces();
    for (const auto& contribution : ActiveForces) {
	contribution->computeForce(system);
    }
}


double ForceCalculator::computePotentialEnergy(const System& system) const {
    double TotalEnergy = 0;
    for (const auto& contribution : ActiveForces) {
    	TotalEnergy += contribution->computeEnergy(system);
    }
    return TotalEnergy;
}

