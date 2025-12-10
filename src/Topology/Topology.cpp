#include "Topology/Topology.hpp"
#include <Eigen/Dense>
#include <iostream>
#include "Topology/Geometry/Bond.hpp"
#include "Topology/Geometry/Angular.hpp"
#include "Topology/Geometry/Dihedral.hpp"


// Implémentation du Constructeur
Topology::Topology(std::vector<Bond> Bonds_in,
                   std::vector<Angular> Angulars_in,
                   std::vector<Dihedral> Dihedrals_in)
    : Bonds(Bonds_in), Angulars(Angulars_in), Dihedrals(Dihedrals_in)
{}

// Getters
const std::vector<Bond>& Topology::getBonds() const { 
    return Bonds; 
}

const std::vector<Angular>& Topology::getAngulars() const { 
    return Angulars; 
}

const std::vector<Dihedral>& Topology::getDihedrals() const { 
    return Dihedrals; 
}


// Setters
void Topology::setBonds(std::vector<Bond> newBonds) { 
    Bonds = std::move(newBonds); // Utiliser std::move est plus efficace
}

void Topology::setAngulars(std::vector<Angular> newAngulars) { 
    Angulars = std::move(newAngulars); 
}

void Topology::setDihedrals(std::vector<Dihedral> newDihedrals) { 
    Dihedrals = std::move(newDihedrals); 
}

