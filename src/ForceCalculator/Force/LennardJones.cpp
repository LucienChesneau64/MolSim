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

    // 2. Correction de Coupure (Tail Correction)
    // Cette correction est nécessaire pour compenser l'énergie potentielle perdue
    // au-delà du rayon de coupure R_c.
    
    // NOTE: Pour simplifier, nous utilisons ici un modèle générique qui suppose
    // que tous les epsilons et sigmas sont similaires (approchés par la moyenne).
    // Une implémentation plus rigoureuse nécessiterait de sommer les corrections
    // par type d'interaction.

    // Calcul des moyennes (Approximation pour la correction de queue)
    //double avg_epsilon = 0.0; 
    //double avg_sigma_6 = 0.0;
    
    // (A implémenter: Calculer les moyennes ou utiliser les valeurs par défaut du système)
    // Pour l'exemple, supposons que nous utilisons des valeurs moyennes :
    // avg_epsilon = system.getAverageEpsilon();
    // avg_sigma = system.getAverageSigma();
    
    //double R_c = std::sqrt(CutoffSquared);
    //double R_c_inv_3 = 1.0 / (R_c * R_c * R_c);
    //double R_c_inv_9 = R_c_inv_3 * R_c_inv_3 * R_c_inv_3;
    
    // Formule simplifiée de correction de queue (pour le potentiel):
    // E_tail = N^2 * (8 * PI / 9) * rho^2 * epsilon * [ (sigma/R_c)⁹ - 3 * (sigma/R_c)³ ]
    // Le terme suivant est une version souvent utilisée, incluant la densité.
    
    // Supposons une correction de queue déjà précalculée pour simplifier
    //double correction_factor = 0.0; // À implémenter avec les paramètres réels.
    
    // total_potential_energy += correction_factor * Constants::AVOGADRO_SCALING_FACTOR;
    
    return total_potential_energy*10;

}



