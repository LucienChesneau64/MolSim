#pragma once

#include <memory>
#include <iostream>
#include <Eigen/Dense>
#include "Atom/Atom.hpp"
#include "Topology/Topology.hpp"
#include "System/System.hpp"
#include "ForceContribution/ForceContribution.hpp"

/**
 * @brief Calcule, met à jour les forces totales agissant sur chaque atome.
 */
class ForceCalculator {
private:
	std::vector<std::unique_ptr<ForceContribution>> ActiveForces;
    
public:
    // --------------------------------------
    // Constructeur
    // --------------------------------------
    ForceCalculator();


    // --------------------
    // getter
    // --------------------

    const std::vector<std::unique_ptr<ForceContribution>>& getActiveForces() const;

    // --------------------
    // Méthodes
    // --------------------
    
    // ajoute une force a ActiveForce
    void addForce(std::unique_ptr<ForceContribution> newForce);

    // compute toutes les forces de ActiveForces
    void computeAllForces(System& system);

    // compute l'energie potentielle du systeme
    double computePotentialEnergy(const System& system) const;
};
