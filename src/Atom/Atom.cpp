#include "Atom/Atom.hpp"

/**
 * @brief Atome définit par son
 * identité: id, sa masse, sa charge, son epsilon, sigma
 * état: postion, vitesse, accélération
 */

// -----------------------------------------------------
// Constructeur
// -----------------------------------------------------
Atom::Atom(int id_in,
           std::string Type_in,
           double Mass_in,
           double Charge_in,
           double Sigma_in,
           double Epsilon_in,
           Eigen::Vector3d Force_in,
           Eigen::Vector3d Position_in,
           Eigen::Vector3d Speed_in,
           Eigen::Vector3d Acceleration_in) :
    id(id_in),
    Type(Type_in),
    Mass(Mass_in),
    Charge(Charge_in),
    Sigma(Sigma_in),
    Epsilon(Epsilon_in),
    Force(Force_in),
    Position(Position_in),
    Speed(Speed_in),
    Acceleration(Acceleration_in) 
{}

// ----------------------------------------------------
// Getters
// ----------------------------------------------------

int Atom::getId() const { return id; }
std::string Atom::getType() const { return Type; }
double Atom::getMass() const { return Mass; }
double Atom::getCharge() const { return Charge; }
double Atom::getSigma() const { return Sigma; }
double Atom::getEpsilon() const { return Epsilon; }

const Eigen::Vector3d& Atom::getForce() const { return Force; }
const Eigen::Vector3d& Atom::getPosition() const { return Position; }
const Eigen::Vector3d& Atom::getSpeed() const { return Speed; }
const Eigen::Vector3d& Atom::getAcceleration() const { return Acceleration; }


// ----------------------------------------------------
// Setters
// ----------------------------------------------------

void Atom::setForce(const Eigen::Vector3d& newForce) { this->Force = newForce; }
void Atom::setPosition(const Eigen::Vector3d& newPosition) { this->Position = newPosition; }
void Atom::setSpeed(const Eigen::Vector3d& newSpeed) { this->Speed = newSpeed; }
void Atom::setAcceleration(const Eigen::Vector3d& newAcceleration) { this->Acceleration = newAcceleration; }



// ----------------------------------------------------
// Méthodes
// ----------------------------------------------------


// add
void Atom::addForce(const Eigen::Vector3d& sumForce) {Force += sumForce; }
void Atom::addPosition(const Eigen::Vector3d& sumPosition) {Position += sumPosition; }
void Atom::addSpeed(const Eigen::Vector3d& sumSpeed) {Speed += sumSpeed; }
void Atom::addAcceleration(const Eigen::Vector3d& sumAcceleration) {Acceleration += sumAcceleration; }

// Énergie Cinétique : E_c = 0.5 * m * |v|^2
double Atom::computeKinEnergy() const {
    return 0.5 * Mass * Speed.squaredNorm();    
}


