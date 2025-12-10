#include <iostream>
#include <vector>
#include <memory>
#include <array>
#include <cmath>
#include <random>
#include <ctime>
#include <numeric>

// Inclusions des classes validées
#include "System/System.hpp"
#include "ForceCalculator/ForceCalculator.hpp"
#include "Integrator/Verlet.hpp"
#include "DynamicEngine/DynamicEngine.hpp"
#include "SystemLoader/SystemLoader.hpp"
#include "Topology/Topology.hpp"
#include "ForceCalculator/ForceContribution/Forces/LennardJones.hpp"
#include "ForceCalculator/ForceContribution/Forces/HarmonicBond.hpp"
#include "OutputManager/OutputManager.hpp"
#include "OutputManager/DataWriter/Output/TrajectoryWriter.hpp" 
#include "OutputManager/DataWriter/Output/LogWriter.hpp" 

// Définitions des constantes de simulation
constexpr double DT_PS = 0.001;               // Pas de temps
constexpr double SIM_TIME_PS = 10;        // Durée totale...
constexpr double EXPORT_INTERVAL_PS = 0.01;    // Intervalle de sauvegarde en ps

// Rayon de coupure pour les forces non-liées (e.g., Lennard-Jones)
constexpr double LJ_CUTOFF_NM = 10.0; 

int main() {
    std::cout << "🚀 Démarrage du Moteur de Dynamique Moléculaire (DM)" << std::endl;
    

    // --- PARAMÈTRES ET CHEMINS DE FICHIERS ---
    const std::string INPUT_FILE = "input_structure.xyz";
    const std::string TRAJECTORY_OUTPUT_FILE = "trajectory_output.xyz"; // Renommé pour plus de clarté
    const std::string LOG_OUTPUT_FILE = "simulation_log.dat"; // NOUVEAU : Fichier pour le log E_pot, T...

    // --- 1. CHARGEMENT ET INITIALISATION DU SYSTÈME ---
    System mol_system; 
    
    try {
        std::cout << "1. Chargement du système et initialisation des vitesses..." << std::endl;
        mol_system = SystemLoader::loadSystemFromXYZ(INPUT_FILE); 
        
    } catch (const std::runtime_error& e) {
        std::cerr << "FATAL ERROR lors du chargement: " << e.what() << std::endl;
        return 1;
    }

    if (mol_system.getParticles().empty()) {
          std::cerr << "FATAL ERROR: Le système n'a pas été initialisé correctement (particules vides)." << std::endl;
          return 1;
    }

    std::cout << "DEBUG: Epsilon de l'atome 0: " << mol_system.getParticles()[0].getEpsilon() << std::endl;

    // --- 2. CONFIGURATION DES COMPOSANTS ---
    
    // Initialisation de ForceCalculator
    ForceCalculator force_calculator; 
    
    // Ajout des Forces
    auto lj_force = std::make_unique<LennardJones>(LJ_CUTOFF_NM);
    //auto bd_force = std::make_unique<HarmonicBond>();
    force_calculator.addForce(std::move(lj_force));
    //force_calculator.addForce(std::move(bd_force));
    
    // Création de l'intégrateur
    Verlet verlet_integrator(force_calculator);


    // --- 3. CONFIGURATION DE L'OUTPUT ET DE L'ENGINE ---

    // 1. Création de l'OutputManager
    OutputManager output_manager; 
    
    // L'Engine DOIT être créé ici pour posséder les données.
    DynamicEngine engine(std::move(mol_system), 
                         verlet_integrator, 
                         force_calculator, 
                         output_manager, 
                         DT_PS);
    
    // Calcul de la fréquence de sauvegarde
    const int SAVE_FREQUENCY_STEPS = 
        static_cast<int>(std::round(EXPORT_INTERVAL_PS / DT_PS));

    // 2. Création et Ajout du TrajectoryWriter (écriture des positions)
    auto traj_writer = std::make_unique<TrajectoryWriter>(
        engine.getSystem(),
        SAVE_FREQUENCY_STEPS, 
        TRAJECTORY_OUTPUT_FILE
    );
    output_manager.addDataWriter(std::move(traj_writer));

    auto log_writer = std::make_unique<LogWriter>(
        engine.getSystem(),        
        SAVE_FREQUENCY_STEPS,
        LOG_OUTPUT_FILE,
        engine.getForceCalculator(),
        engine.getIntegrator()
    );
    output_manager.addDataWriter(std::move(log_writer));


    // 3. Initialisation des fichiers
    output_manager.filesInitializer(); 

    // --- 4. EXÉCUTION DE LA SIMULATION ---
    
    std::cout << "\n▶️ Démarrage de la simulation (" << SIM_TIME_PS << " ps, sauvegarde tous les " 
              << SAVE_FREQUENCY_STEPS << " pas)..." << std::endl;
    
    // Exécution
    engine.run(SIM_TIME_PS); 
    
    std::cout << "\n✅ Simulation terminée. Trajectoire écrite dans " << TRAJECTORY_OUTPUT_FILE 
              << " et données de log dans " << LOG_OUTPUT_FILE << std::endl;

    return 0;
}
