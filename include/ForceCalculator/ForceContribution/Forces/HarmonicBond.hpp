#pragma once

#include "System/System.hpp"
#include "ForceCalculator/ForceContribution/ForceContribution.hpp"


class HarmonicBond final : public ForceContribution {
	public:
		// constructeur
		HarmonicBond() {}

		// Force
		void computeForce(System& system) override;

		// Energie potentiel

		double computeEnergy(const System& system) const override;
};
