#pragma once

#include <Eigen/Dense>
#include <iostream>
#include <string>

/**
 * @brief Représente une particule (atome) dans la simulation de Dynamique Moléculaire.
 * Stocke les propriétés physiques et les variables d'état (position, vitesse, force, etc.).
 */
class Atom {
private:
    int id;
    std::string Type;
    double Mass;
    double Charge;
    
    // Paramètres de Van Der Walls (VDW)
    double Sigma;
    double Epsilon;
    
    // Variables d'état
    Eigen::Vector3d Force;
    Eigen::Vector3d Position;
    Eigen::Vector3d Speed;
    Eigen::Vector3d Acceleration;
    
public:
    // -------------------------------------<
    // Constructeur
    // -------------------------------------
    Atom(int id,
         std::string Type,
         double Mass, 
         double Charge,
         double Sigma,
         double Epsilon,
         Eigen::Vector3d Force,
         Eigen::Vector3d Position,
         Eigen::Vector3d Speed,
         Eigen::Vector3d Acceleration);
    // --------------------------------------
    // Getters
    // --------------------------------------
    // get constantes
    int getId() const;
    std::string getType() const;
    double getMass() const;
    double getCharge() const;
    double getSigma() const;
    double getEpsilon() const;
    
    // get variables états
    const Eigen::Vector3d& getForce() const;
    const Eigen::Vector3d& getPosition() const;
    const Eigen::Vector3d& getSpeed() const;
    const Eigen::Vector3d& getAcceleration() const;
    
    // ---------------------------------------
    // Setters
    // ---------------------------------------
    
    // set variables états
    void setForce(const Eigen::Vector3d& newForce); 
    void setPosition(const Eigen::Vector3d& newPosition);
    void setSpeed(const Eigen::Vector3d& newSpeed);
    void setAcceleration(const Eigen::Vector3d& newAcceleration);
    // ----------------------------------------
    // Méthodes
    // ----------------------------------------
    
    // add
    void addForce(const Eigen::Vector3d& sumForce);
    void addPosition(const Eigen::Vector3d& sumPosition);
    void addSpeed(const Eigen::Vector3d& sumSpeed);
    void addAcceleration(const Eigen::Vector3d& sumAcceleration);

    // Compute Energy
    double computeKinEnergy() const;
};
