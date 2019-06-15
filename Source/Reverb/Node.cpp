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
		// waveVector = new float[numberOfOtherNodes];
		// scatteringMatrix
		// scatteringMatrix = new float[numberOfOtherNodes * numberOfOtherNodes]; // K x K scattering matrix
		
		// for(int i = 0; i < numberOfOtherNodes * numberOfOtherNodes; i++) {
		//
		// 	scatteringMatrix[i] = 2.0 / (float) numberOfOtherNodes; // isotropic scattering matrix
		//
		// 	if(i % (numberOfOtherNodes + 1) == 0) scatteringMatrix[i] -= 1.0; // apply identity subtraction along diagonal
		//
		// }
		
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
	
	// 	void Node::gatherInputWaveVectorFromNodes() { // get inputs from all terminals
	// 		for(int terminal = 0; terminal < terminalCount; terminal ++) {
	// //			waveVector[terminal] = 0;
	// 			waveVector[terminal] = terminals[terminal]->read();
	// 		}
	// 	}
	//
	// 	void Node::distributeOutputWaveVectorToNodes() {
	// 		for(int terminal = 0; terminal < terminalCount; terminal++) {
	// 			terminals[terminal]->write(waveVector[terminal]);
	// 		}
	// 	}
	
	void Node::prepareInput(float *inputWaveVector, float sourceInput){
		float networkInput = sourceInput / numberOfOtherNodes;
		
		for(int terminal = 0; terminal < numberOfOtherNodes; terminal++) {
			inputWaveVector[terminal] = terminals[terminal]->read() + networkInput;
		}
	}
	
	void Node::distributeOutput(float *outputWaveVector){
		float outs[numberOfOtherNodes];
		
		output = 0.0;
		for(int terminal = 0; terminal < numberOfOtherNodes; terminal++) {
			outs[terminal] = outputWaveVector[terminal] * absorptionFactor;
			terminals[terminal]->write(outs[terminal]);
			output += outs[terminal];
		}
	}
	
	float Node::getNodeOutput() {
		return output;
	}
	
	
	void Node::scatter(float sourceInput) {
		
		float inputWaveVector[numberOfOtherNodes]; // assign temporary vector for calculation
		
		prepareInput(inputWaveVector, sourceInput);
		
		float outputWaveVector[numberOfOtherNodes]; // assign temporary vector for calculation
		
		float sum = 0.0;		
		for(int node = 0; node < numberOfOtherNodes; node++) {
			sum += inputWaveVector[node];
		}
		sum *= 2;
		sum /= numberOfOtherNodes;
		
		for(int node = 0; node < numberOfOtherNodes; node++) {
			outputWaveVector[node] = sum - inputWaveVector[node];
		}
	
		distributeOutput(outputWaveVector);
	}
	
	void Node::setAbsorption(const float amount) {
		absorptionFactor = amount;
	}
}

