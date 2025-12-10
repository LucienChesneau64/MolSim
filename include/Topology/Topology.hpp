#pragma once

#include <vector>
#include "Topology/Geometry/Bond.hpp" 
#include "Topology/Geometry/Angular.hpp" 
#include "Topology/Geometry/Dihedral.hpp" 


class Topology {
private:
    std::vector<Bond> Bonds;
    std::vector<Angular> Angulars;
    std::vector<Dihedral> Dihedrals;
    
public:
    // Constructeur par défaut
    Topology() = default;

    // Constructeur
    Topology(std::vector<Bond> Bonds,
             std::vector<Angular> Angulars, 
             std::vector<Dihedral> Dihedrals);

    // Getters
    const std::vector<Bond>& getBonds() const;
    const std::vector<Angular>& getAngulars() const;
    const std::vector<Dihedral>& getDihedrals() const;
    
    // Setters
    void setBonds(std::vector<Bond> newBonds);
    void setAngulars(std::vector<Angular> newAngulars);
    void setDihedrals(std::vector<Dihedral> newDihedrals);
};
