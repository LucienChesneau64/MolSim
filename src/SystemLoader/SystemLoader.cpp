#include "SystemLoader/SystemLoader.hpp"
#include "Atom/Atom.hpp"
#include "System/System.hpp"
#include "Topology/Topology.hpp"
#include "MathUtils/minimumImageDistance.hpp" // Nécessaire pour déterminer les liaisons avec PBC
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <random>
#include <ctime>
#include <numeric>
#include <iomanip>
#include <stdexcept>
#include <array>
#include <algorithm> 


constexpr double R_GAS = 0.00831446; // Constante des gaz (kJ/mol/K)
constexpr double TARGET_TEMP = 300.0;

struct AtomParams {
    double mass;
    double charge;
    double sigma;
    double epsilon;
};

// Base de Données Statique des Paramètres d'Atomes (Champ de Force Simplifié)
static const std::map<std::string, AtomParams> ATOM_PARAMETERS = {
    // Type : {Masse (u), Charge (e), Sigma (nm), Epsilon (kJ/mol)}
    {"H", {1.008, 0.0, 0.250, 0.19}}, 
    {"C", {12.011, 0.0, 0.340, 0.46}}, 
    {"O", {15.999, 0.0, 0.300, 0.65}}, 
    {"N", {14.007, 0.0, 0.325, 0.71}}, 
    {"AR", {39.948, 0.0, 0.340, 0.998}},
    {"X", {100.0, 0.0, 0.1, 0.1}} 
};


// =========================================================================
// Fonctions utilitaires de création et de déduction (privées)
// =========================================================================

Atom SystemLoader::createDefaultAtom(int id, const std::string& type_raw) {
    // 1. Nettoyer et normaliser le type
    std::string clean_type;
    std::stringstream ss(type_raw);
    ss >> clean_type;
    std::transform(clean_type.begin(), clean_type.end(), clean_type.begin(), ::toupper);
    
    // Simplification : Préférer le premier caractère si le type n'est pas trouvé (e.g., C1 -> C)
    // NOTE: C'est un compromis pour un chargeur simple.
    if (clean_type.length() > 1 && ATOM_PARAMETERS.find(clean_type) == ATOM_PARAMETERS.end()) {
        clean_type = clean_type.substr(0, 1);
    }
    
    // 2. Recherche et gestion de l'inconnu
    auto it = ATOM_PARAMETERS.find(clean_type);
    
    if (it == ATOM_PARAMETERS.end()) {
        std::cerr << "AVERTISSEMENT: Type d'atome inconnu: " << type_raw 
                  << " (Recherche: '" << clean_type << "'). Utilisation des paramètres 'X' par défaut." << std::endl;
        it = ATOM_PARAMETERS.find("X");
    }

    const AtomParams& params = it->second;
    Vector3D zero_vec = Vector3D::Zero();

    return Atom(id, it->first, params.mass, params.charge, params.sigma, params.epsilon,
                zero_vec, zero_vec, zero_vec, zero_vec);
}


/**
 * @brief Détermine les angles à partir de la liste des liaisons.
 */
std::vector<Angular> SystemLoader::determineAngles(const std::vector<Atom>& particles, const std::vector<Bond>& bonds) {
    std::vector<Angular> angles;
    double K_angle_default = 500.0; 
    double Theta0_default = 1.91986; // 110 degrés en radians

    std::vector<std::vector<int>> connectivity(particles.size());
    for (const Bond& bond : bonds) {
        const auto& couple = bond.getCouple();
        int i = couple[0]; // Utilisation de getCouple()
        int j = couple[1];
        
        connectivity[i].push_back(j);
        connectivity[j].push_back(i);
    }

    std::cout << "-> Deducing angles from connectivity..." << std::endl;

    for (size_t k = 0; k < particles.size(); ++k) { // k est l'atome central
        const auto& neighbors = connectivity[k];

        for (size_t idx_i = 0; idx_i < neighbors.size(); ++idx_i) {
            for (size_t idx_j = idx_i + 1; idx_j < neighbors.size(); ++idx_j) {
                int i = neighbors[idx_i];
                int j = neighbors[idx_j];

                // L'angle est formé par i - k - j
                Angular new_angle({i, (int)k, j}, K_angle_default, Theta0_default);
                angles.push_back(new_angle);
            }
        }
    }
    return angles;
}

/**
 * @brief Détermine la vélocité initiale à partir des caractéristiques des atomes (Maxwell-Boltzmann).
 * REQUIERT Atom::computeKineticEnergy() pour être valide.
 */
void SystemLoader::initializeVelocities(System& system) {
    auto& particles = system.getParticles();
    
    static std::mt19937 generator(static_cast<unsigned int>(std::time(nullptr)));

    double total_mass = 0.0;
    Vector3D total_momentum = Vector3D::Zero(); 
    
    // --- 1. Tirage des vitesses (Distribution Maxwell-Boltzmann) ---
    for (Atom& atom : particles) {
        double mass = atom.getMass(); 
        total_mass += mass;
        
        // σ = sqrt(R * T / M)
        double stddev = std::sqrt(R_GAS * TARGET_TEMP / mass); 

        std::normal_distribution<> dist_v(0.0, stddev); 
        
        Vector3D vel(dist_v(generator), dist_v(generator), dist_v(generator));
        atom.setSpeed(vel);
        
        total_momentum += mass * vel;
    }

    // --- 2. Correction de la Vitesse du Centre de Masse (CM) ---
    Vector3D V_CM = total_momentum / total_mass;

    for (Atom& atom : particles) {
        atom.setSpeed(atom.getSpeed() - V_CM);
    }
    
    // --- 3. Mise à l'Échelle Finale de la Température ---
    // Recalculer l'énergie cinétique après la correction du CM
    double current_kinetic_energy = 0.0;
    for (const Atom& atom : particles) {
        current_kinetic_energy += atom.computeKinEnergy(); 
    }
    
    double num_atoms = particles.size();
    // Degrés de liberté = 3 * N_atomes - 3 (pour la correction CM)
    double N_DOF = 3.0 * num_atoms - 3.0; 
    
    // T = 2 * K / (N_DOF * R_GAS)
    double current_temp = 2.0 * current_kinetic_energy / (N_DOF * R_GAS);

    // Lambda = sqrt(T_cible / T_actuelle)
    double scale_factor = std::sqrt(TARGET_TEMP / current_temp);

    // Appliquer le facteur d'échelle à toutes les vitesses
    for (Atom& atom : particles) {
        atom.setSpeed(atom.getSpeed() * scale_factor);
    }
}

// ---------------------------------------------
// ------------ systemLoadeerfromXYZ -----------
// ---------------------------------------------

System SystemLoader::loadSystemFromXYZ(const std::string& filename) {

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("ERREUR: Impossible d'ouvrir le fichier .xyz: " + filename);
    }

    std::vector<Atom> particles;
    std::string line;
    int num_atoms_expected = 0;
    Vector3D box_dimensions = Vector3D::Zero();
    
    // ------------------------------------------
    // Ligne 1: Nombre total d'atomes
    // ------------------------------------------
    if (!std::getline(file, line)) throw std::runtime_error("ERREUR: Fichier .xyz vide.");
    try {
        num_atoms_expected = std::stoi(line);
    } catch (...) {
        throw std::runtime_error("ERREUR: Nombre d'atomes invalide dans le .xyz: '" + line + "'");
    }

    if (num_atoms_expected <= 0) {
        throw std::runtime_error("ERREUR: Le nombre d'atomes attendu est zéro ou négatif.");
    }

    // ------------------------------------------
    // Ligne 2: Ligne de commentaire (Doit contenir les dimensions de la boîte)
    // ------------------------------------------
    if (!std::getline(file, line)) throw std::runtime_error("ERREUR: Fichier .xyz tronqué (manque la ligne de commentaire).");
    
    // Tenter de lire les dimensions de la boîte (Lx Ly Lz) de la ligne de commentaire.
    // Nous cherchons trois nombres flottants.
    std::stringstream ss_box(line);
    double lx, ly, lz;
    if (ss_box >> lx >> ly >> lz) {
        box_dimensions = Vector3D(lx, ly, lz);
        std::cout << "Successfully loaded box dimensions: " << lx << " " << ly << " " << lz << " from line 2." << std::endl;
    } else {
        // Option B: Définir une boîte par défaut si elle n'est pas trouvée (ex: 10x10x10 nm)
        box_dimensions = Vector3D(5.0, 5.0, 5.0);
        std::cerr << "AVERTISSEMENT: Impossible de lire les dimensions de la boîte. Défaut: 5.0 nm." << std::endl;
    }

    // ------------------------------------------
    // Lignes suivantes: Lecture des atomes (Type X Y Z)
    // ------------------------------------------
    for (int atom_count = 0; atom_count < num_atoms_expected; ++atom_count) {
        if (!std::getline(file, line)) {
            throw std::runtime_error("ERREUR: Fichier .xyz tronqué avant la lecture de l'atome " + std::to_string(atom_count + 1));
        }

        std::stringstream ss(line);
        std::string atom_type_raw;
        double x, y, z;
        
        // Lecture de type (ex: H), X, Y, Z séparés par des espaces
        if (ss >> atom_type_raw >> x >> y >> z) {
            // Création de l'atome via la fonction utilitaire existante
            Atom new_atom = createDefaultAtom(atom_count, atom_type_raw);
            new_atom.setPosition(Vector3D(x, y, z));
            particles.push_back(std::move(new_atom));

        } else {
            std::cerr << "ERREUR FATALE: Ligne d'atome mal formatée (ligne " << atom_count + 3 << "): " << line << std::endl;
            throw std::runtime_error("Ligne d'atome mal formatée dans le fichier XYZ.");
        }
    }
    
    std::cout << "Successfully loaded " << particles.size() << " particles from " << filename << std::endl;

    // ------------------------------------------
    // 3. Post-traitement: Déduction de la Topologie et Initialisation
    // ------------------------------------------
    
    // Construction de la Topologie
    Topology final_topology(
		    {},
		    {},
        {}
    );

    std::cout << "Topology ready: " << final_topology.getBonds().size() << " bonds, "
              << final_topology.getAngulars().size() << " angles." << std::endl;

    // Construction et Initialisation du Système
    System loaded_system(std::move(particles),
                         std::move(final_topology),
                         box_dimensions
           ); 

    initializeVelocities(loaded_system);

    return loaded_system;
}


