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
	Node::Node(Point position, int numberOfOtherNodes) : position(position), numberOfOtherNodes(numberOfOtherNodes)
	{
		waveVector = new float[numberOfOtherNodes];
		scatteringMatrix = new float[numberOfOtherNodes*numberOfOtherNodes];
		
		for(int i = 0; i < numberOfOtherNodes * numberOfOtherNodes; i++) {
			
			scatteringMatrix[i] = 2.0 / (float) numberOfOtherNodes;

			if(i % (numberOfOtherNodes + 1) == 0) scatteringMatrix[i] -= 1.0;
			
		}
		
	}
	
	Point Node::getPosition()
	{
		return position;
	}
	
	void Node::setPosition(Point p)
	{
		position.setY(p.getY());
		position.setX(p.getX());
		position.setZ(p.getZ());
	}
	
	void Node::addTerminal(SDN::Terminal *terminal)
	{
		terminals[terminalCount] = terminal;
		terminalCount++;
	}
	
	void Node::gatherInputWaveVectorFromNodes() {
		for(int terminal = 0; terminal < terminalCount; terminal ++) {
			waveVector[terminal] = 0;
			waveVector[terminal] = terminals[terminal]->read();
		}
	}
	
	void Node::distributeOutputWaveVectorToNodes() {
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
		
		float networkInput = sourceInput / numberOfOtherNodes;
		
		float tempVector[numberOfOtherNodes];
	
		for(int i = 0; i < numberOfOtherNodes; i ++) {
			tempVector[i] = 0.0;
			for(int j = 0; j < numberOfOtherNodes; j++) {
				tempVector[i] += scatteringMatrix[(numberOfOtherNodes * i) + j] * (networkInput + waveVector[j]);
			}
			tempVector[i] *= absorptionFactor;
		}
		
		for(int i = 0; i < numberOfOtherNodes; i ++) {
			waveVector[i] = tempVector[i];
		}
	}
}
