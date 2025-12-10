#include "OutputManager/DataWriter/Output/LogWriter.hpp"
#include <iomanip>
#include <iostream>

// ---------------------------------------------------
// Constructeur / Destructeur
// --------------------------------------------------
LogWriter::LogWriter(const System& sys, int freq, const std::string& filename,
                     ForceCalculator& fc, Integrator& sch)
    : CalculationWriter(sys, freq, fc, sch), filename_(filename) {}

LogWriter::~LogWriter() {
    if (fileStream_.is_open()) {
        fileStream_.close();
        std::cout << "Fichier de log/données fermé : " << filename_ << std::endl; 
    }
}

// ---------------------------------------------------
// initializeFile : Ouvre le flux et écrit l'en-tête CSV
// ---------------------------------------------------

void LogWriter::initializeFile() {
    fileStream_.open(filename_, std::ios::out | std::ios::trunc);
    if (!fileStream_.is_open()) {
        std::cerr << "ERREUR: Impossible d'ouvrir le fichier de log: " << filename_ << std::endl;
    } else {
        // Écriture de l'en-tête
        fileStream_ << "# Time(ps) Step E_pot(kJ/mol) E_kin(kJ/mol) Temp(K)\n";
    }
}

// ---------------------------------------------------
// writeData : Point d'entrée appelé par OutputManager
// ---------------------------------------------------

void LogWriter::writeData(double time, int step) {
    // 1. Vérification de la fréquence (Frequency_ est un membre hérité)
    if (step % Frequency == 0) { 
        if (fileStream_.is_open()) {
            writeCSVFrame(time, step); 
        }
    }
}

// ---------------------------------------------------
// writeCSVFrame : Implémentation du format tabulaire
// ---------------------------------------------------

void LogWriter::writeCSVFrame(double current_time, int current_step) {
    // Vérification de l'état du flux
    if (!fileStream_.good()) return;

    // --- CALCUL DES PROPRIÉTÉS ---
    // Les membres forceCal_ et scheme_ sont maintenant disponibles grâce à l'héritage de CalculationWriter.
    
    // 1. Énergie Potentielle : Appel à la méthode de la ForceCalculator
    double e_pot = forceCal_.computePotentialEnergy(system);
    // 2. Énergie Cinétique :
    double e_kin = system.computeKineticEnergy();
    
    // 3. Température : 
    const double KB_KJ_PER_MOL_K = 8.31446E-3; // R en kJ/mol/K
    const double N_ATOMS = system.getParticles().size(); // 5
    const double N_DOF = 3.0 * N_ATOMS; // 15 degrés de liberté

    double temp = (2.0 * e_kin) / (N_DOF * KB_KJ_PER_MOL_K); // Calcul de T

    // ---------------------------------------------------

    // Formatage : Utilisation de std::setw pour aligner les colonnes (tabulaire)
    fileStream_ << std::fixed
            << std::setprecision(6)
            << std::setw(10) << current_time << " " 
            << std::setw(6) << current_step << " ";

// --- ZONE ENERGIE (Utiliser SCIENTIFIC pour la robustesse) ---
// Largeur de colonne pour les nombres scientifiques (20 est très sûr)
const int ENERGY_COL_WIDTH = 20;

fileStream_ << std::scientific // Appliquer SCIENTIFIC pour les énergies
            << std::setprecision(8)
            << std::setw(ENERGY_COL_WIDTH) << e_pot << " "    // E_pot
            << std::setw(ENERGY_COL_WIDTH) << e_kin << " ";   // E_kin
            
// --- TEMPÉRATURE (Peut rester en FIXED) ---
fileStream_ << std::fixed 
            << std::setprecision(4)
            << std::setw(10) << temp 
            << "\n";
}
