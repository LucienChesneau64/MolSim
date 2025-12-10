#pragma once

#include "OutputManager/DataWriter/DataWriter.hpp"
#include "ForceCalculator/ForceCalculator.hpp"
#include "Integrator/Integrator.hpp"

// Classe intermédiaire qui ajoute les outils de calcul à l'interface d'écriture
class CalculationWriter : public DataWriter {
protected:
    // Les membres sont hérités de DataWriter (const System&, int Frequency_)
    // Nous ajoutons les références aux outils de calcul :
    ForceCalculator& forceCal_; 
    Integrator& scheme_; 

public:
    // Le constructeur prend tous les arguments et les distribue
    CalculationWriter(const System& sys, int freq, 
                      ForceCalculator& fc, Integrator& sch) 
        // Assurez-vous que DataWriter a un constructeur simple : DataWriter(const System&, int)
        : DataWriter(sys, freq), forceCal_(fc), scheme_(sch) {}
    
    // Il n'est pas nécessaire de déclarer d'autres méthodes virtuelles ici,
    // car elles sont déjà dans DataWriter.
    virtual ~CalculationWriter() = default;
};
