/*
  ==============================================================================

    Connection.cpp
    Created: 15 Mar 2019 4:10:21pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Connection.h"
#include <iostream>

namespace SDN{
	Connection::Connection(float distance, float sampleRate, float maxDistance) : sampleRate(sampleRate)
	{
		float delay = (sampleRate * distance / SDN::c);
		startToEndDelay = ModulatingDelay::fromDistance(sampleRate, distance, maxDistance);
		endToStartDelay = ModulatingDelay::fromDistance(sampleRate, distance, maxDistance);
		
		startTerminal = new SDN::Terminal(endToStartDelay, startToEndDelay);
		endTerminal = new SDN::Terminal(startToEndDelay, endToStartDelay);
	}
	
	void Connection::setLength(float distance) // used for updating the delay lines
	{
		startToEndDelay->setDelayLengthFromDistance(distance);
		endToStartDelay->setDelayLengthFromDistance(distance);
	}
	
	Connection::Connection(SDN::Node startNode, SDN::Node endNode, float sampleRate, float maxDistance) :
	Connection(startNode.getPosition().distanceTo(endNode.getPosition()), sampleRate, maxDistance) {}
	
	SDN::Terminal* Connection::getStartTerminal(){
		return startTerminal;
	}
	
	SDN::Terminal* Connection::getEndTerminal(){
		return endTerminal;
	}
}
