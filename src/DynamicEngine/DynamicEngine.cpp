#include "DynamicEngine/DynamicEngine.hpp"
#include <iostream>
#include <utility>

// --------------------------------------------------
// Constructor
// --------------------------------------------------
DynamicEngine::DynamicEngine(
    System Systeme_in,
    Integrator& Scheme_in,
    ForceCalculator& ForceCal_in,
    OutputManager& OutputMgr_in,
    double dt_in)
    : Systeme(std::move(Systeme_in)), 
      Scheme(Scheme_in), 
      ForceCal(ForceCal_in), 
      OutputMgr(OutputMgr_in),
      dt(dt_in) {}
// ---------------------------------------------------
// Méthode
// ---------------------------------------------------
void DynamicEngine::run(double T) {

    double time = 0.0;
    int step = 0;
    
    // Initialisation: Calculer les forces initiales avant la première étape d'intégration
    ForceCal.computeAllForces(Systeme);
    
    OutputMgr.writeDatas(time, step);

    // Boucle de simulation corrigée
    while (time < T) {
        // 1. Exécuter l'étape d'intégration
        // met à jour Systeme.Particles (positions, vitesses, forces, acceleration)
        Scheme.Step(Systeme, dt); 

	// 2. Écriture des données
        // L'OutputManager vérifie la fréquence lui-même pour chaque DataWriter.
        OutputMgr.writeDatas(time, step);

        // 3. add time and step.
        time += dt;
	step++;
    }
    
}
