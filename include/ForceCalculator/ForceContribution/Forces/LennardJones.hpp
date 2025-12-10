#include "System/System.hpp"
#include "ForceCalculator/ForceContribution/ForceContribution.hpp"
#include <Eigen/Dense>

using Vector3D = Eigen::Vector3d;

class LennardJones final : public ForceContribution {
private:
    double CutoffSquared;

public:
    LennardJones(double cutoff) : CutoffSquared(cutoff * cutoff) {}
    
    void computeForce(System& system) override; 
    double computeEnergy(const System& system) const override; 
};


