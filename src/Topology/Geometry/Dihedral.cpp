#include "Topology/Geometry/Dihedral.hpp"

Dihedral::Dihedral(std::array<int, 4> Indices_in, double K_phi_in, double n_in, double delta_in)
    : Indices(Indices_in), K_phi(K_phi_in), n(n_in), Delta(delta_in) 
{}



const std::array<int, 4>& Dihedral::getIndices() const {
    return Indices;
}

double Dihedral::getK_phi() const {
    return K_phi;
}

double Dihedral::getn() const {
    return n;
}

double Dihedral::getDelta() const {
    return Delta;
}
