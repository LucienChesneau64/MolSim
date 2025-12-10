#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "OutputManager/DataWriter/DataWriter.hpp"

class OutputManager {
	private:
		std::vector<std::unique_ptr<DataWriter>> dataManager;
	public:
		// constructeur
		OutputManager() = default;

		// methods
		void addDataWriter(std::unique_ptr<DataWriter> type); 
		void filesInitializer(); // à voir si pas nécessaire.
		void writeDatas(double time, int step);
};
