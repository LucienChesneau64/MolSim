#pragma once

#include <iostream>
#include "System/System.hpp"


class DataWriter {
	protected:
		int Frequency;
		const System& system;
	public:
		// constructeur
		DataWriter(const System& system_, int Frequency_):
			Frequency(Frequency_),
			system(system_) {};
		// destructeur
		virtual ~DataWriter() = default;

		// methods
		virtual void initializeFile() = 0;

		virtual void writeData(double time, int step) = 0;
};

