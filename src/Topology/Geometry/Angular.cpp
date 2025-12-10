#include "Topology/Geometry/Angular.hpp"

// Implémentation du Constructeur
Angular::Angular(std::array<int, 3> Triplet_in, double K_angle_In, double Theta0_In)
    : Triplet(Triplet_in), K_angle(K_angle_In), Theta0(Theta0_In) 
{
}

// Getters
const std::array<int, 3>& Angular::getTriplet() const {
    return Triplet;
}

double Angular::getK_angle() const {
    return K_angle;
}

double Angular::getTheta0() const {
    return Theta0;
}
