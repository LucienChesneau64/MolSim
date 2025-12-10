#pragma once

#include <array>

/**
 * @brief Représente une interaction de force d'angle harmonique (Angle Force) impliquant trois atomes.
 * Stocke les indices des atomes, la constante de force (K_angle) et l'angle d'équilibre (Theta0).
 */
class Angular {
private:
    // Indices des 3 atomes impliqués (i-k-j, k est le centre)
    std::array<int, 3> Triplet;
    
    // Paramètres de force
    double K_angle;  // Constante de force (K_theta)
    double Theta0;   // Angle d'équilibre (theta0)
    
public:
    // Constructeur
    Angular(std::array<int, 3> Triplet, double K_angle_in, double Theta0_in);
    
    // Getters
    const std::array<int, 3>& getTriplet() const;
    double getK_angle() const;
    double getTheta0() const;
};
