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
	
	void Node::scatter(float sourceInput, float *waveVector) {
	
		float absorptionFactor = 0.7;
		
		float networkInput = sourceInput / delayOrder;
		
		float tempVector[delayOrder];
	
		for(int i = 0; i < delayOrder; i ++) {
			for(int j = 0; j < delayOrder; j++) {
				tempVector[i] += scatteringMatrix[delayOrder*i + j] * (networkInput + waveVector[j]);
			}
			tempVector[i] *= absorptionFactor;
		}
		
		for(int i = 0; i < delayOrder; i ++) {
			waveVector[i] = tempVector[i];
		}
	}
}
