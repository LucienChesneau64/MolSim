#include "Integrator/Verlet.hpp"
#include <iostream>
#include <cmath>
using Vector3D = Eigen::Vector3d;


// ----------------------------
// Methods
// ----------------------------
void Verlet::Step(System& system, double dt) {
    
    std::vector<Atom>& particles = system.getParticles();
    double dt_sq_half = 0.5 * dt * dt;
    
    // ETAPE 1: Mise à jour des positions et de la vitesse à mi-pas
    for (Atom& atom : particles) {
        
        const Vector3D& current_speed = atom.getSpeed();
        const Vector3D& current_acceleration = atom.getAcceleration();
        
        // 1. Mise à jour de la Position (r(t+dt))
        // r(t+dt) = r(t) + (v(t)*dt) + (0.5*a(t)*dt^2)
        atom.addPosition(current_speed * dt + current_acceleration * dt_sq_half);
        
        // 2. Mise à jour de la Vitesse à dt/2 (v(t+dt/2))
        // v(t+dt/2) = v(t) + (0.5*a(t)*dt)
        atom.addSpeed(current_acceleration * 0.5 * dt); 
    }
    // 3. Mise à jour de la Position pour conditions au bord périodique
    system.wrapPositions();
    
    // ETAPE 2: Calcul des Forces F(t+dt)
    
    force_calculator.computeAllForces(system);
    
    // ETAPE 3: Mise à jour de la vitesse et de l'accélération finales
    for (Atom& atom : particles) {
        
        const Vector3D& F_new = atom.getForce();
        double mass = atom.getMass();
        
        // 1. Nouvelle Accélération (a(t+dt) = F(t+dt) / m)
        Vector3D new_acceleration = F_new / mass;
        
        // 2. Vitesse (v(t+dt) = v(t+dt/2) + 0.5*a(t+dt)*dt) 
        atom.addSpeed(new_acceleration * 0.5 * dt);
        
        // 3. Mise à jour de l'Accélération
        atom.setAcceleration(new_acceleration);
    }
}
