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
		
		for(int node = 0; node < nodeCount; node++)
		{
			sourceToNodeDelays[node] = SDN::Delay(sampleRate, source.distanceTo(nodes[node].getPosition()));
			nodeToMicDelays[node] = SDN::Delay(sampleRate, mic.distanceTo(nodes[node].getPosition()));
		}
				
		sourceMicDelay = new SDN::Delay(sampleRate, source.distanceTo(mic));
	}
	
	float Network::scatter(float in)
	{
		in *= 0.5;
		
		sourceMicDelay->write(in);
		
		for(int node = 0; node < nodeCount; node++)
		{
			sourceToNodeDelays[node].write(in);
			nodes[node].gatherInputWaveVector();
		}
		
		for(int node = 0; node < nodeCount; node++)
		{
			nodes[node].scatter(sourceToNodeDelays[node].read());
			nodes[node].distributeOutputWaveVector();
			nodeToMicDelays[node].write(nodes[node].getNodeOutput());
		}
		
		auto out = sourceMicDelay->read()/source.distanceTo(mic);
		for(int node = 0; node < nodeCount; node++)
		{
			out += nodeToMicDelays[node].read() / (1 + (mic.distanceTo(nodes[node].getPosition()) / (source.distanceTo(nodes[node].getPosition()))));
		}
		return out;
	}
}
