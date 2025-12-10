#include "Topology/Geometry/Bond.hpp"

// Constructor
Bond::Bond(std::array<int, 2> Couple_in, double K_bond_in, double r0_in)
    : Couple(Couple_in), K_bond(K_bond_in), r0(r0_in) 
{}

// getters
const std::array<int, 2>& Bond::getCouple() const {
    return Couple;
}

double Bond::getK_bond() const {
    return K_bond;
}

double Bond::getr0() const {
    return r0;
}
