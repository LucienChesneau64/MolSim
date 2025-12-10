#pragma once

#include <array>

/**
 * @brief Représente une interaction de torsion (dièdre) impliquant quatre atomes.
 * * Stocke les indices des atomes et les paramètres de la fonction cosinus (un seul terme).
 */
class Dihedral {
private:
    // Indices des 4 atomes (i-k-l-j)
    std::array<int, 4> Indices; 
    
    // Paramètres de force pour un terme V_dihedral = K_phi * (1 + cos(n*phi - delta))
    double K_phi;   // Amplitude de la force
    double n;       // Périodicité (multiplicateur d'angle)
    double Delta;   // Angle de phase
    
public:
    // Constructeur
    Dihedral(std::array<int, 4> Indices, double K_phi_in, double n_in, double delta_in);
    
    // Méthodes d'accès (Getters) pour lire les paramètres
    const std::array<int, 4>& getIndices() const;
    double getK_phi() const;
    double getn() const;
    double getDelta() const;
};
