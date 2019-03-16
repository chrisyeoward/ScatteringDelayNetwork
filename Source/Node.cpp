/*
  ==============================================================================

    Node.cpp
    Created: 11 Mar 2019 10:23:28pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Node.h"
#include <iostream>

namespace SDN {
	Node::Node(Point position) : position(position)
	{
		scatteringMatrix = new float[delayOrder*delayOrder];
		
		for(int i = 0; i < delayOrder * delayOrder; i++) {
			scatteringMatrix[i] = 2.0 / (float) delayOrder;

			if(i % (delayOrder + 1) == 0) scatteringMatrix[i] -= 1.0;
		}
		
	}
	
	Point Node::getPosition()
	{
		return position;
	}
	
	void Node::addTerminal(SDN::Terminal *terminal)
	{
		terminals[terminalCount] = terminal;
		terminalCount++;
	}
	
	void Node::gatherInputWaveVector() {
		for(int terminal = 0; terminal < terminalCount; terminal ++) {
			waveVector[terminal] = 0;
			waveVector[terminal] = terminals[terminal]->read();
		}
	}
	
	void Node::distributeOutputWaveVector() {
		for(int terminal = 0; terminal < terminalCount; terminal++) {
			terminals[terminal]->write(waveVector[terminal]);
		}
	}
	
	float Node::getNodeOutput() {
		float out = 0.0;
		
		for(int terminal = 0; terminal < terminalCount; terminal ++) {
			out += waveVector[terminal];
		}
		return out;
	}
	
	void Node::scatter(float sourceInput) {
	
		float absorptionFactor = 0.7;
		
		float networkInput = sourceInput / delayOrder;
		
		float tempVector[delayOrder];
	
		for(int i = 0; i < delayOrder; i ++) {
			tempVector[i] = 0.0;
			for(int j = 0; j < delayOrder; j++) {
				tempVector[i] += scatteringMatrix[(delayOrder * i) + j] * (networkInput + waveVector[j]);
			}
			tempVector[i] *= absorptionFactor;
		}
		
		for(int i = 0; i < delayOrder; i ++) {
			waveVector[i] = tempVector[i];
		}
	}
}
