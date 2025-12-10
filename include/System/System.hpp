#ifndef MOLSIMGUI_SYSTEM_HPP
#define MOLSIMGUI_SYSTEM_HPP

#include "Atom/Atom.hpp"
#include "Topology/Topology.hpp"
#include <vector>
#include <array>
#include <iostream>
#include <Eigen/Dense>

/**
 * @brief Représente l'ensemble du système de Dynamique Moléculaire.
 */

class System {
private:
    std::vector<Atom> Particles;
    Topology Interactions;
    Eigen::Vector3d DimBox;
    
public:
    // ---------------------------------------------------
    // Constructeur (Correction du nom Cutoff)
    // ---------------------------------------------------
    
    System() = default; // constructeur par défaut

    System(std::vector<Atom> particles_in, 
           Topology Interactions_in,
           Eigen::Vector3d dimBox_in);
    
    // ---------------------------------------------------
    // Getters
    // ---------------------------------------------------
    
    // 1. Getter modifiable (pour l'intégrateur). NE DOIT PAS être const.
    std::vector<Atom>& getParticles();

    // 2. Getter constant (pour l'exportateur, les calculs de force). DOIT être const.
    const std::vector<Atom>& getParticles() const; 

    // Pour la topologie (lecture seule)
    const Topology& getTopology() const;
    
    // Pour les dimensions de la boîte (lecture seule)
    const Eigen::Vector3d& getDimBox() const;

    
    // ---------------------------------------------------
    // Setters (Déclarations correctes)
    // ---------------------------------------------------
    void setParticles(const std::vector<Atom>& newParticles);
    void setTopology(const Topology& newTopology);
    void setDimBox(const Eigen::Vector3d& newDimBox);

    // ---------------------------------------------------
    // Methods
    // ---------------------------------------------------
    void wrapPositions();
    void resetForces();
    double computeKineticEnergy() const;
};

#endif
