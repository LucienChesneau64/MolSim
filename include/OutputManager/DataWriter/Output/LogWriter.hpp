#pragma once

#include <fstream>
#include <string>
#include <iostream>

#include "OutputManager/DataWriter/DataWriter.hpp" 
#include "OutputManager/DataWriter/CalculationWriter.hpp" 

class LogWriter : public CalculationWriter {
private:
    std::ofstream fileStream_; 
    std::string filename_;
    
    void writeCSVFrame(double current_time, int current_step);

public:
    LogWriter(const System& sys, int freq, const std::string& filename,
              ForceCalculator& fc, Integrator& sch);

    void initializeFile() override;
    void writeData(double time, int step) override;
    
    ~LogWriter() override; 
};
