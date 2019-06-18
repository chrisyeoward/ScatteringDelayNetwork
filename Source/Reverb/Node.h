/*
 ==============================================================================
 
 Node.h
 Created: 11 Mar 2019 10:23:28pm
 Author:  Chris Yeoward
 
 ==============================================================================
 */

#pragma once
#include "Point.h"
#include "Terminal.h"
#include <string.h>
#include "Constants.h"
#include "Filter.h"
#include "../JuceLibraryCode/JuceHeader.h"


/*
 This represents a scattering junction.
 */

namespace SDN {
	class Node {
	private:
		float absorptionFactor = 0.85;
		Point position;
		int numberOfOtherNodes;
		int terminalCount = 0;
		static const int maxTerminalCount = MAXIMUM_NODES - 1;
		SDN::Terminal *terminals[maxTerminalCount]; // hold references to each of the other nodes
		
		float output = 0.0;
		float filterOutputForTerminal(float output, int terminal);
		Filter* filters;
		
	public:
		void prepareInput(float* inputWaveVector, float sourceInput);
		void distributeOutput(float* outputWaveVector);
		
		void gatherInputWaveVectorFromNodes(); // accumulates samlpes into the wave vector from each of the connection terminals
		void distributeOutputWaveVectorToNodes(); // writes the wave vector to each of the respective terminals
		float getNodeOutput(); // get the sum of all outputs at the current time step
		
		void addTerminal(SDN::Terminal *terminal); // add another terminal
		void scatter(float sourceInput); // scatters the current wave vector
		Point getPosition();
		void setPosition(Point p); // used for updating the position and nodes
		
		void setAbsorption(const float amount);
		
		Node () {};
		Node(Point position, int numberOfOtherNodes);
		~Node() {};
		
	};
}

