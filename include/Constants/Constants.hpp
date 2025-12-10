#pragma once

#include <cmath>

/**
 * @brief Fichier centralisant toutes les constantes physiques et les facteurs d'échelle.
 */

namespace Constants {

    // =========================================================================
    // 1. CONSTANTES PHYSIQUES DE BASE
    // =========================================================================

    // Constante de Boltzmann (kB)
    // Utilisée pour convertir T (K) en énergie (kJ/mol) ou pour le Thermostat
    // kB = 1.380649e-23 J/K
    // Conversion en kJ/(mol*K) : (1.380649e-23 J/K) * (6.02214076e23 mol^-1) * (1e-3 kJ/J)
    constexpr double KB_KJ_PER_MOL_K = 0.008314462618; 

    // Nombre d'Avogadro (NA)
    constexpr double N_AVOGADRO = 6.02214076e23; // mol^-1

    // =========================================================================
    // 2. FACTEUR D'ÉCHELLE CRITIQUE (Unités du Système)
    // =========================================================================

    /**
     * @brief Facteur de conversion pour les Forces et l'Énergie.
     * * Conversion nécessaire : [kJ/mol * nm^-1] (Force potentielle) -> [u * nm / ps^2] (Force Newtonienne)
     * Facteur = 1 / (NA * 1000)
     * Où NA convertit 'par mole' en 'par particule' et 1000 convertit kJ en J.
     * La valeur est : 1 / (6.02214076e23 * 1000) ≈ 1.6605e-27
     */
    constexpr double AVOGADRO_SCALING_FACTOR = 1.660538921e-27;

    // =========================================================================
    // 3. PARAMÈTRES NUMÉRIQUES
    // =========================================================================

    // Tolérance numérique (pour SHAKE, intégrateurs, tests de singularité)
    constexpr double EPSILON_TOL = 1e-12; 

} // namespace Constants
