#pragma once

#include <utility>
#include "OutputManager/OutputManager.hpp"

void OutputManager::writeDatas(double time, int step) {
	for (const auto& writer : dataManager) {
		writer->writeData(time, step);
	}
}


void OutputManager::addDataWriter(std::unique_ptr<DataWriter> type) {

	dataManager.push_back(std::move(type));
}


void OutputManager::filesInitializer() {
    for (const auto& writer : dataManager) {
        writer->initializeFile(); 
    }
}


