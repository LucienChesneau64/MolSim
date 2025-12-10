#pragma once

#include "System/System.hpp"
#include "Topology/Topology.hpp"
#include "Atom/Atom.hpp"
#include "Topology/Geometry/Bond.hpp"
#include "Topology/Geometry/Angular.hpp"
#include <vector>
#include <string>
#include <array>
#include <Eigen/Dense>

using Vector3D = Eigen::Vector3d;

class SystemLoader {
public:
    /**
     * @brief Lit un fichier GROMACS (.gro), crée les atomes et déduit la topologie.
     * Les coordonnées dans le fichier .gro sont en nanomètres (nm).
     * @param filename Le chemin vers le fichier .gro.
     * @param max_bond_dist Le rayon maximum pour déduire une liaison (en nm, ex: 0.18 pour 1.8 Å).
     * @return Un objet System entièrement initialisé.
     */

    static System loadSystemFromXYZ(const std::string& filename);

private:
    // Fonctions de construction et de déduction
    static Atom createDefaultAtom(int id, const std::string& type);

    static std::vector<Bond> determineBonds(const std::vector<Atom>& particles,
                                            double max_bond_dist,
                                            const Vector3D& dim_box);

    static std::vector<Angular> determineAngles(const std::vector<Atom>& particles, const std::vector<Bond>& bonds);
    static void initializeVelocities(System& system);
};
