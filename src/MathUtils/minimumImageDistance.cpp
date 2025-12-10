#include "MathUtils/minimumImageDistance.hpp"

Vector3D minimumImageDistance(const Vector3D& r1, const Vector3D& r2, const Vector3D& boxSize) {
    Vector3D r_ij = r1 - r2; // Distance brute

    for (int k = 0; k < 3; ++k) {
        double L_k = boxSize[k];
        double L_half = L_k * 0.5;
         
        // Si la distance est supérieure à L/2, ramenez-la de L
        if (r_ij[k] > L_half) {
            r_ij[k] -= L_k;
        }
        // Si la distance est inférieure à -L/2, ramenez-la de +L
        else if (r_ij[k] < -L_half) {
            r_ij[k] += L_k;
        }
    }
    // r_ij est maintenant le vecteur distance le plus court dans le cube central
    return r_ij;
}
