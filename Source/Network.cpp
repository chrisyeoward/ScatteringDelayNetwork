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
		
		for(int node = 0; node < nodeCount; node++)
		{
			connectionCount += node;
			nodes[node] = SDN::Node(bounds[node].getScatteringNodePosition(mic, source), delayOrder);
		}
		
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
	
	StereoOutput Network::scatterStereo(float in)
	{
		scatter(in);
		
		StereoOutput out;
		
		float fromSource = sourceMicDelay->read()/source.distanceTo(mic);
		float sourceAzimuth = source.azimuthFrom(mic);
		float sinAzimuth = sin(sourceAzimuth);
		float denom = 1 / sqrt(2 * (1 + pow(sinAzimuth, 2)));
		float sourceGainLeft = (1 - sinAzimuth) * denom;
		float sourceGainRight = (1 + sinAzimuth) * denom;
		
		out.L = fromSource * sourceGainLeft;
		out.R = fromSource * sourceGainRight;
		
		for(int node = 0; node < nodeCount; node++)
		{
			float fromSource = nodeToMicDelays[node].read();
			fromSource /= (1 + (mic.distanceTo(nodes[node].getPosition()) / (source.distanceTo(nodes[node].getPosition()))));
			float sourceAzimuth = nodes[node].getPosition().azimuthFrom(mic);
			float sinAzimuth = sin(sourceAzimuth);
			float denom = 1 / sqrt(2 * (1 + pow(sinAzimuth, 2)));
			float sourceGainLeft = (1 - sinAzimuth) * denom;
			float sourceGainRight = (1 + sinAzimuth) * denom;
			
			out.L += fromSource * sourceGainLeft;
			out.R += fromSource * sourceGainRight;
		}
		
		return out;
	}
	
	float Network::scatterMono(float in)
	{
		scatter(in);
		auto out = sourceMicDelay->read()/source.distanceTo(mic);
		for(int node = 0; node < nodeCount; node++)
		{
			out += nodeToMicDelays[node].read() / (1 + (mic.distanceTo(nodes[node].getPosition()) / (source.distanceTo(nodes[node].getPosition()))));
		}
		return out;
	}
	
	void Network::scatter(float in)
	{
		in *= 0.5;
		
		sourceMicDelay->write(in);
		
		for(int node = 0; node < nodeCount; node++)
		{
			sourceToNodeDelays[node].write(in);
			nodes[node].gatherInputWaveVectorFromNodes();
		}
		
		for(int node = 0; node < nodeCount; node++)
		{
			nodes[node].scatter(sourceToNodeDelays[node].read());
			nodes[node].distributeOutputWaveVectorToNodes();
			nodeToMicDelays[node].write(nodes[node].getNodeOutput());
		}
	}
	
	void Network::setSourcePosition(float x, float y, float z) {
		std::cout << "Network - Setting source position... \n";
		source.setX(x);
		source.setY(y);
		
		for(int node = 0; node < nodeCount; node++)
		{
			nodes[node].setPosition(bounds[node].getScatteringNodePosition(mic, source));
		}
	}
}
