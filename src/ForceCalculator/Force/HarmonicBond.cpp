#include "ForceCalculator/ForceContribution/Forces/HarmonicBond.hpp"
#include "Constants/Constants.hpp"

#include <Eigen/Dense> 
using Vector3D = Eigen::Vector3d;


/**
 * @brief Calcule et ajoute les forces de liaison harmoniques.
 * Formule: F = - K_b * (r - r0) * u_ij
 */
void HarmonicBond::computeForce(System& system) {
    
    std::vector<Atom>& particles = system.getParticles();
    const Topology& topo = system.getTopology();
    
    for (const Bond& bond : topo.getBonds()) {
        const std::array<int, 2>& indices = bond.getCouple();
        int idx_i = indices[0];
        int idx_j = indices[1];

        const Vector3D& position_i = particles[idx_i].getPosition();
        const Vector3D& position_j = particles[idx_j].getPosition();

        Vector3D r_ij = position_i - position_j;
        
        double r_sq = r_ij.squaredNorm(); 
        if (r_sq < 1e-10) continue; 
        double r = std::sqrt(r_sq);

        // Magnitude scalaire de la force (F_ij = - K_b * (r - r0))
        double F_ij_magnitude = -bond.getK_bond() * (r - bond.getr0());
        
        // Facteur Scalaire (S = F_ij / r)
        double S = F_ij_magnitude / r;
        
        // Vecteur force F_i (F_i = S * r_ij)
        Vector3D F_i = S * r_ij;
	    F_i *= Constants::AVOGADRO_SCALING_FACTOR;

        // Appliquer les forces
        particles[idx_i].addForce(F_i);
        particles[idx_j].addForce(-F_i);
    }
}

double HarmonicBond::computeEnergy(const System& system) const {
    
    double total_potential_energy = 0.0;
    
    const std::vector<Atom>& particles = system.getParticles();
    const Topology& topo = system.getTopology();
    
    for (const Bond& bond : topo.getBonds()) {
        const std::array<int, 2>& indices = bond.getCouple();
        int idx_i = indices[0];
        int idx_j = indices[1];

        const Vector3D& position_i = particles[idx_i].getPosition();
        const Vector3D& position_j = particles[idx_j].getPosition();

        // Calcul du vecteur de distance et de la magnitude r
        Vector3D r_ij = position_i - position_j;
        double r_sq = r_ij.squaredNorm();
        
        if (r_sq < 1e-10) continue; 
        double r = std::sqrt(r_sq);

        double delta_r = r - bond.getr0();
        
        // Calcul de l'énergie potentielle E = 0.5 * K_b * (r - r0)^2
        double E_bond = 0.5 * bond.getK_bond() * delta_r * delta_r;
        
        total_potential_energy += E_bond;
    }
    return total_potential_energy;
}


