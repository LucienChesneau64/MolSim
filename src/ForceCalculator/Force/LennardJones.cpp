#include "ForceCalculator/ForceContribution/Forces/LennardJones.hpp"
#include "MathUtils/minimumImageDistance.hpp"
#include "Constants/Constants.hpp"



void LennardJones::computeForce(System& system) {
    
    std::vector<Atom>& particles = system.getParticles();
    size_t N = particles.size();
    const Vector3D& DimBox = system.getDimBox();
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            

	    Vector3D r_ij = minimumImageDistance(particles[i].getPosition(), particles[j].getPosition(), DimBox);
            double r_sq = r_ij.squaredNorm();	
	   
            if (r_sq < 1e-4) continue; 
    
            double r_inv_sq = 1.0 / r_sq;
            
            // Paramètres
            double sigma_ij = (particles[i].getSigma() + particles[j].getSigma()) * 0.5;


            double epsilon_ij = std::sqrt(particles[i].getEpsilon() * particles[j].getEpsilon());

            // Termes r⁻⁶ et r⁻¹²
            double sigma_over_r_sq = sigma_ij * sigma_ij * r_inv_sq;
            double sigma_over_r6 = sigma_over_r_sq * sigma_over_r_sq * sigma_over_r_sq;
            double sigma_over_r12 = sigma_over_r6 * sigma_over_r6;
            
            // Facteur Scalaire
	    double S = (24.0 * epsilon_ij * r_inv_sq) * (2.0 * sigma_over_r12 - sigma_over_r6);           
            // Vecteur Force
            Vector3D F_i = S * r_ij;

	    F_i *= 10;

            // Accumulation des forces
            particles[i].addForce(F_i);
            particles[j].addForce(-F_i);
        }
    }
}


/**
 * @brief Calcule l'énergie potentielle totale de Lennard-Jones (LJ) avec correction de coupure.
 * @param system Le système moléculaire actuel.
 * @return L'énergie potentielle totale des interactions LJ.
 */
double LennardJones::computeEnergy(const System& system) const {
    
    double total_potential_energy = 0.0;
    const std::vector<Atom>& particles = system.getParticles();
    size_t N = particles.size();
    const Vector3D& DimBox = system.getDimBox();

    // 1. Calcul de l'énergie entre paires
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            
            // Calcul de la distance minimale périodique
            Vector3D r_ij = minimumImageDistance(particles[i].getPosition(), particles[j].getPosition(), DimBox);
            double r_sq = r_ij.squaredNorm();   

            // Application de la coupure (CutoffSquared est un membre de la classe)
            if (r_sq >= CutoffSquared) {
                continue; 
            }

            if (r_sq < 1e-10) continue; 
    
            double r_inv_sq = 1.0 / r_sq;
            
            // --- Calcul des paramètres LJ (Mixage de Lorentz-Berthelot) ---
            double sigma_ij = (particles[i].getSigma() + particles[j].getSigma()) * 0.5;
            double epsilon_ij = std::sqrt(particles[i].getEpsilon() * particles[j].getEpsilon());

            // --- Calcul des termes r⁻⁶ et r⁻¹² ---
            double sigma_over_r_sq = sigma_ij * sigma_ij * r_inv_sq;
            double sigma_over_r6 = sigma_over_r_sq * sigma_over_r_sq * sigma_over_r_sq;
            double sigma_over_r12 = sigma_over_r6 * sigma_over_r6;
            
            // --- Calcul de l'Énergie LJ (E = 4 * epsilon * (r⁻¹² - r⁻⁶)) ---
            double E_pair = 4.0 * epsilon_ij * (sigma_over_r12 - sigma_over_r6);
            
            // Mise à l'échelle
	    
            total_potential_energy += E_pair;
        }
    }
   
    return total_potential_energy*10;

}



