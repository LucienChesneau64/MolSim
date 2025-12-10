#pragma once

#include <array>

/**
 * @brief Représente une interaction de liaison harmonique (Bond) entre deux atomes.
 * Stocke les indices des atomes, la constante de force (Kb) et la distance d'équilibre (r0).
 */
class Bond {
private:
    // Indices des 2 atomes impliqués (i-j)
    std::array<int, 2> Couple;
    double K_bond;  // Constante de force (Kb)
    double r0;      // Distance d'équilibre
    
public:
    // ----------------------------------------------
    // Constructor
    // ----------------------------------------------
    Bond(std::array<int, 2> Couple, double K_bond_in, double r0_in);
    
    // -----------------------------------------------
    // Getters
    // -----------------------------------------------
    const std::array<int, 2>& getCouple() const;
    double getK_bond() const;
    double getr0() const;

};
