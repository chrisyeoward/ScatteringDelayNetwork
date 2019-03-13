/*
  ==============================================================================

    Node.cpp
    Created: 11 Mar 2019 10:23:28pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Node.h"

namespace SDN {
	Node::Node(Point position) : position(position)
	{}
	
	Point Node::getPosition()
	{
		return position;
	}
	
	void Node::scatter(float sourceInput, float *waveVector) {
		int delayOrder = 2;
		float absorptionFactor = 0.8;
		
		float networkInput = sourceInput / delayOrder;
		
		float inputs[delayOrder];
		memcpy(inputs, waveVector, delayOrder * sizeof(float));
		
//		float scatteringMatrix[2][2] = {{0,0},{0,0}};
		//		A = 2/K 1 1T - 1; 1 = [1;1] 1T = [1,1]
		waveVector[0] = absorptionFactor * (networkInput + inputs[1]);
		waveVector[1] = absorptionFactor * (networkInput + inputs[0]);
//		float outs[] = {absorptionFactor * (sourceInput + inputs[1]), absorptionFactor * (sourceInput + inputs[0])};
//		return *outs;
		
//		return 0.0;
	}
}
