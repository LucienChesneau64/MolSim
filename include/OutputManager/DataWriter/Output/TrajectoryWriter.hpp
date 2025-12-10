#include <fstream>
#include <string>
#include "OutputManager/DataWriter/DataWriter.hpp"
#include "System/System.hpp"
#include <Eigen/Dense>

class TrajectoryWriter : public DataWriter {
private:
    std::ofstream fileStream_;
    std::string filename_;
    
    // Fonction privée interne pour écrire un seul cadre XYZ
    void writeXYZFrame(double current_time);

public:
    // Constructeur hérite de DataWriter
    TrajectoryWriter(const System& sys, int freq, const std::string& filename);

    // Implémentations virtuelles
    void initializeFile() override;
    void writeData(double time, int step) override;
    
    // Destructeur
    ~TrajectoryWriter() override;
};
