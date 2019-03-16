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
	Connection::Connection(float distance, float sampleRate) : distance(distance)
	{
//		std::cout << "connection\n";
//		std::cout << distance;
//		std::cout << "\n";
		startToEndDelay = new SDN::Delay(sampleRate, distance);
		endToStartDelay = new SDN::Delay(sampleRate, distance);
		
		startTerminal = new SDN::Terminal(endToStartDelay, startToEndDelay);
		endTerminal = new SDN::Terminal(startToEndDelay, endToStartDelay);
	}
	
	Connection::Connection(SDN::Node startNode, SDN::Node endNode, float sampleRate) :
	Connection(startNode.getPosition().distanceTo(endNode.getPosition()), sampleRate) {}
	
	SDN::Terminal* Connection::getStartTerminal(){
		return startTerminal;
	}
	
	SDN::Terminal* Connection::getEndTerminal(){
		return endTerminal;
	}
}
