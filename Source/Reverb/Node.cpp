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
		filters = new Filter[numberOfOtherNodes];
		const int filterOrder = 2;
//		float a[filterOrder + 1] = {1.0, -2.7618, 2.5368, -0.7749}; // carpet
//		float b[filterOrder + 1] = {0.6876, -1.9207, 1.7899, -0.5567};
		
//		float a[filterOrder + 1] = {1.0, -1.8540, 0.8455};
//		float b[filterOrder + 1] = {0.1684, -0.2432, 0.0748};
		
		float a[filterOrder + 1] = {1, -2e-16, 0.171572875253810}; //butter
		float b[filterOrder + 1] = {0.2929, 0.5858, 0.2929};
		
//		float a[filterOrder + 1] = {1.0, -1.8588, 0.8590};
//		float b[filterOrder + 1] = {0.9874, -1.817, 0.8392};
		
		for(int i = 0; i < numberOfOtherNodes; i++){
			filters[i].prepare(filterOrder);
			filters[i].setCoefficients(a, b);
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
	
	void Node::prepareInput(float *inputWaveVector, float sourceInput)
	{
		float networkInput = sourceInput / numberOfOtherNodes;
		
		for(int terminal = 0; terminal < numberOfOtherNodes; terminal++) {
			inputWaveVector[terminal] = terminals[terminal]->read() + networkInput;
		}
	}
	
	float Node::filterOutputForTerminal(float output, int terminal)
	{
//		return filters[terminal].processSample(output);
		return output * absorptionFactor;
	}
	
	void Node::distributeOutput(float *outputWaveVector)
	{
		float outs[numberOfOtherNodes];
		
		output = 0.0;
		for(int terminal = 0; terminal < numberOfOtherNodes; terminal++) {
			outs[terminal] = filterOutputForTerminal(outputWaveVector[terminal], terminal);
			terminals[terminal]->write(outs[terminal]);
			output += outs[terminal];
		}
	}
	
	float Node::getNodeOutput()
	{
		return output;
	}
	
	void Node::scatter(float sourceInput)
	{
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
	
	void Node::setAbsorption(const float amount)
	{
		absorptionFactor = amount;
	}
}

