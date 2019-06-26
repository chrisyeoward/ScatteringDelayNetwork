/*
  ==============================================================================

    Terminal.cpp
    Created: 15 Mar 2019 5:00:32pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Terminal.h"

namespace SDN {
	
	Terminal::Terminal(SDN::Delay* readDelay, SDN::Delay* writeDelay) : readDelay(readDelay), writeDelay(writeDelay) {};
	
	void Terminal::write(float sample) {
		writeDelay->write(sample);
	}
	
	float Terminal::read() {
		return readDelay->readWithAirAbsorption();
	}
}
