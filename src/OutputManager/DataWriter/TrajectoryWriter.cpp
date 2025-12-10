#include "OutputManager/DataWriter/Output/TrajectoryWriter.hpp" 
#include "System/System.hpp" 
#include <iostream>
#include <iomanip>

// ---------------------------------------------------
// Constructeur / Destructeur (Identique à l'implémentation GRO/texte)
// ---------------------------------------------------

TrajectoryWriter::TrajectoryWriter(const System& sys, int freq, const std::string& filename)
    : DataWriter(sys, freq), filename_(filename) {}

TrajectoryWriter::~TrajectoryWriter() {
    if (fileStream_.is_open()) {
        fileStream_.close();
        std::cout << "Fichier de trajectoire XYZ fermé : " << filename_ << std::endl;
    }
}

// ---------------------------------------------------
// initializeFile : Ouvre le flux texte
// ---------------------------------------------------

void TrajectoryWriter::initializeFile() {
    // Le mode std::ios::out | std::ios::trunc garantit un nouveau fichier
    fileStream_.open(filename_, std::ios::out | std::ios::trunc);
    if (!fileStream_.is_open()) {
        std::cerr << "ERREUR: Impossible d'ouvrir le fichier de trajectoire XYZ." << std::endl;
    }
}

// ---------------------------------------------------
// writeData : Point d'entrée appelé par OutputManager
// ---------------------------------------------------

void TrajectoryWriter::writeData(double time, int step) {
    if (step % Frequency == 0) { 
        if (fileStream_.is_open()) {
            writeXYZFrame(time);
            fileStream_ << std::flush;
        }
    }
}

// ---------------------------------------------------
// writeXYZFrame : Implémentation du format XYZ
// ---------------------------------------------------

void TrajectoryWriter::writeXYZFrame(double current_time) {
    
    const std::vector<Atom>& particles = system.getParticles();
    int N = (int)particles.size();
    
    // 1. Ligne N : Nombre total d'atomes
    fileStream_ << N << "\n";
    
    // 2. Ligne de commentaire (optionnel, souvent le temps)
    fileStream_ << "Frame at time = " << std::fixed << std::setprecision(3) << current_time << " ps\n";
    
    // 3. N Lignes de coordonnées (Nom Atome, X, Y, Z)
    fileStream_ << std::fixed << std::setprecision(8); // Définir la précision des coordonnées
    
    for (const auto& atom : particles) {
        const auto& pos = atom.getPosition();
        
        // Nom de l'atome (utilisez un nom générique si vous n'avez pas de type atomique)
        // Note: Assurez-vous que atom.getType() renvoie bien une chaîne de caractères
        std::string atom_name = atom.getType(); 
        
        fileStream_ << std::left << std::setw(4) << atom_name 
                    << std::setw(15) << pos[0] 
                    << std::setw(15) << pos[1] 
                    << std::setw(15) << pos[2] 
                    << "\n"; 
    }
    // NE PAS OUBLIER le \n à la fin de la dernière ligne, sinon MDAnalysis ne lira qu'une seule frame!
}
