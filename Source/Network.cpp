/*
  ==============================================================================

    ScatteringDelay.cpp
    Created: 11 Mar 2019 6:12:29pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Network.h"
#include <iostream>


namespace SDN
{
	Network::Network(float sampleRate)
	{
		connections = new SDN::Connection[connectionCount];
		int connection = 0;
		
		for(int node = 0; node < delayOrder; node++)
		{
			for(int otherNode = node + 1; otherNode < nodeCount; otherNode++)
			{
				connections[connection] = SDN::Connection(nodes[node], nodes[otherNode], sampleRate);
				nodes[node].addTerminal(connections[connection].getStartTerminal());
				nodes[otherNode].addTerminal(connections[connection].getEndTerminal());
				connection++;
			}
		}
				
		sourceMicDelay = new SDN::Delay(sampleRate, source.distanceTo(mic));
		
		sourceNode1Delay = new SDN::Delay(sampleRate, source.distanceTo(nodes[0].getPosition()));
		sourceNode2Delay = new SDN::Delay(sampleRate, source.distanceTo(nodes[1].getPosition()));
		sourceNode3Delay = new SDN::Delay(sampleRate, source.distanceTo(nodes[2].getPosition()));
		
		node1MicDelay = new SDN::Delay(sampleRate, mic.distanceTo(nodes[0].getPosition()));
		node2MicDelay = new SDN::Delay(sampleRate, mic.distanceTo(nodes[1].getPosition()));
		node3MicDelay = new SDN::Delay(sampleRate, mic.distanceTo(nodes[2].getPosition()));
	}
	
	float Network::scatter(float in)
	{
		in *= 0.5;
		
		sourceMicDelay->write(in);
		
		sourceNode1Delay->write(in);
		sourceNode2Delay->write(in);
		sourceNode3Delay->write(in);
		
		for(int node = 0; node < nodeCount; node++)
		{
			nodes[node].gatherInputWaveVector();
		}
		
		nodes[0].scatter(sourceNode1Delay->read());
		nodes[1].scatter(sourceNode2Delay->read());
		nodes[2].scatter(sourceNode3Delay->read());
		
		for(int node = 0; node < nodeCount; node++)
		{
			nodes[node].distributeOutputWaveVector();
		}
		
		node1MicDelay->write(nodes[0].getNodeOutput());
		node2MicDelay->write(nodes[1].getNodeOutput());
		node3MicDelay->write(nodes[2].getNodeOutput());
		
		auto out = sourceMicDelay->read();
		
//		for(int node = 0; node < delayOrder; node++)
//		{
//			nodes[node].scatter(in);
//			nodes[node].distributeOutputWaveVector();
//			out += nodes[node].getNodeOutput();
//		}
		out += node1MicDelay->read();
		out += node2MicDelay->read();
		out += node3MicDelay->read();
		
		return out;
	}
}
