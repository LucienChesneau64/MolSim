#include "System/System.hpp"

// ---------------------------------------------------------
// Constructeur
// ---------------------------------------------------------
System::System(std::vector<Atom> particles_in, 
               Topology interactions_in, 
               Eigen::Vector3d dimBox_in)
    : Particles(std::move(particles_in)), 
      Interactions(std::move(interactions_in)),
      DimBox(dimBox_in) {}

// ----------------------------------------------------
// Getters
// ----------------------------------------------------

std::vector<Atom>& System::getParticles() {return Particles; }
const std::vector<Atom>& System::getParticles() const {return Particles;}
const Topology& System::getTopology() const {return Interactions; }
const Eigen::Vector3d& System::getDimBox() const {return DimBox;}

// ----------------------------------------------------
// Setters
// ----------------------------------------------------

void System::setParticles(const std::vector<Atom>& newParticles) {Particles = newParticles; }
void System::setTopology(const Topology& newInteractions) {Interactions = newInteractions; }
void System::setDimBox(const Eigen::Vector3d& newDimBox) {DimBox = newDimBox; }

// -----------------------------------------------------
// Methods
// -----------------------------------------------------

void System::wrapPositions() {
    const auto& L = this->getDimBox(); // L est [Lx, Ly, Lz]
    
    for (Atom& atom : this->Particles) {
        Eigen::Vector3d pos = atom.getPosition();
        
        for (int k = 0; k < 3; k++) {
            double L_k = L[k];
            
	    pos[k] -= L_k * std::floor(pos[k] / L_k);
        }
        atom.setPosition(pos);
    }
}

void System::resetForces() {
	for (Atom& atom : this->Particles) {
		atom.setForce(Eigen::Vector3d::Zero());
	}
}

double System::computeKineticEnergy() const {
	double kin = 0;
	for (const Atom& atom : this->Particles) {
		kin += atom.computeKinEnergy();	
	}
	return kin;
}
