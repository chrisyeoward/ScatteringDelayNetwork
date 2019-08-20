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
		
//		float a[filterOrder + 1] = {1.0, -1.8540, 0.8455}; // hard 1
//		float b[filterOrder + 1] = {0.1684, -0.2432, 0.0748};
		
		float a[filterOrder + 1] = {1.0, -2e-16, 0.17157}; // butter
		float b[filterOrder + 1] = {0.2929, 0.5858, 0.2929};
		
//		float a[filterOrder + 1] = {1.0, -1.8588, 0.8590}; // hard 2
//		float b[filterOrder + 1] = {0.9874, -1.817, 0.8392};
		
//		float a[filterOrder + 1] = {1.0000, -2.3527, 1.9623, -0.5777}; // my concrete
//		float b[filterOrder + 1] = {0.9514, -2.2356, 1.8631, -0.5480};
		
//		float a[filterOrder + 1] = {1.0000, -3.2485, 4.1247, -2.4180, 0.5507}; // carpet
//		float b[filterOrder + 1] = {0.9324, -3.0241, 3.8350, -2.2456, 0.5109};
		
		
		for(int i = 0; i < numberOfOtherNodes; i++){
			filters[i].prepare(filterOrder);
			filters[i].setCoefficients(a, b);
		}
		
		weightingFactor = 2.0 / (float) numberOfOtherNodes;
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
		return absorptionFactor * output; // * filters[terminal].processSample(output);
//		return output * absorptionFactor;
	}
	
	void Node::distributeOutput(float *outputWaveVector)
	{
		float outs[numberOfOtherNodes];
		
		output = 0.0;
		for(int terminal = 0; terminal < numberOfOtherNodes; terminal++) {
			outs[terminal] = filterOutputForTerminal(outputWaveVector[terminal], terminal);
			terminals[terminal]->write(outs[terminal]);
			output += outs[terminal]; // weighting vector
		}
//		output *= weightingFactor;
	}
	
	float Node::getNodeOutput()
	{
		return output;
	}
	
	void Node::scatter(float sourceInput)
	{
		float inputWaveVector[numberOfOtherNodes]; // assign temporary vector for calculation
		
		prepareInput(inputWaveVector, sourceInput);
		
		float sum = 0.0;		
		for(int node = 0; node < numberOfOtherNodes; node++) {
			sum += inputWaveVector[node];
		}
		sum *= weightingFactor;
		
		float outputWaveVector[numberOfOtherNodes]; // assign temporary vector for calculation
		
		for(int node = 0; node < numberOfOtherNodes; node++) {
			outputWaveVector[node] = sum - inputWaveVector[node];
		}
	
		distributeOutput(outputWaveVector);
	}
	
	void Node::setAbsorption(const float alpha)
	{
		absorptionFactor = sqrt(1 - alpha);
	}
}

