#pragma once

#include "System/System.hpp"

class ForceContribution {
	public:
	// constructeur
	virtual ~ForceContribution() = default;
	// methods
	
	/**
	 * @brief Calcule la force et l'ajoute a chaque atome dans le système.
	 */ 
	virtual void computeForce(System&) = 0;
	/**
	 * @brief Calcule l'energie potentiel du système.
	 */
	virtual double computeEnergy(const System& system) const = 0;
};

