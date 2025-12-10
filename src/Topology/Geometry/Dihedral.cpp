#include "Topology/Geometry/Dihedral.hpp"

// Implémentation du Constructeur :
// Utilise la liste d'initialisation pour assigner les valeurs aux membres
Dihedral::Dihedral(std::array<int, 4> Indices_in, double K_phi_in, double n_in, double delta_in)
    : Indices(Indices_in), K_phi(K_phi_in), n(n_in), Delta(delta_in) 
{}



// Implémentation des Getters :
// Retourne une référence constante à l'array pour éviter la copie
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
