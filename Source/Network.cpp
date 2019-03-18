/*
  ==============================================================================

    ScatteringDelay.cpp
    Created: 11 Mar 2019 6:12:29pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Network.h"

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
		for(int node = 0; node < nodeCount - 1; node++)
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
			sourceToNodeDelays[node] = *Delay::fromDistance(sampleRate, source.distanceTo(nodes[node].getPosition()));
			nodeToMicDelays[node] = *Delay::fromDistance(sampleRate, mic.distanceTo(nodes[node].getPosition()));
		}
				
		sourceMicDelay = Delay::fromDistance(sampleRate, source.distanceTo(mic));
	}
	
	StereoOutput Network::scatterStereo(float in)
	{
		scatter(in);
		
		StereoOutput out;
		
		float fromSource = sourceMicDelay->read()/(fmax(source.distanceTo(mic), 0.1));
		float sourceAzimuth = source.azimuthFrom(mic);
		float sinAzimuth = sin(sourceAzimuth);
		float denom = 1 / sqrt(2 * (1 + pow(sinAzimuth, 2)));
		float sourceGainLeft = (1 - sinAzimuth) * denom;
		float sourceGainRight = (1 + sinAzimuth) * denom;
		
		out.L = fromSource * sourceGainLeft;
		out.R = fromSource * sourceGainRight;
		
		for(int node = 0; node < nodeCount; node++)
		{
			float fromNode = nodeToMicDelays[node].read();
			fromNode /= (1 + (fmax(mic.distanceTo(nodes[node].getPosition()), 0.1) / (fmax(source.distanceTo(nodes[node].getPosition()), 0.1))));
			float nodeAzimuth = nodes[node].getPosition().azimuthFrom(mic);
			float sinAzimuth = sin(nodeAzimuth);
			float denom = 1 / sqrt(2 * (1 + pow(sinAzimuth, 2)));
			float nodeGainLeft = (1 - sinAzimuth) * denom;
			float nodeGainRight = (1 + sinAzimuth) * denom;
			
			out.L += fromNode * nodeGainLeft;
			out.R += fromNode * nodeGainRight;
		}
		
		return out;
	}
	
	float Network::scatterMono(float in)
	{
		scatter(in); // accommodate 10x gain factor in 1/r volume adjustment
		auto out = sourceMicDelay->read()/source.distanceTo(mic);
		for(int node = 0; node < nodeCount; node++)
		{
			out += nodeToMicDelays[node].read() / (1 + (mic.distanceTo(nodes[node].getPosition()) + 1 / (source.distanceTo(nodes[node].getPosition()) + 1)));
		}
		return out;
	}
	
	void Network::scatter(float in)
	{
		in *= 0.1;  // accommodate 10x gain factor in 1/r volume adjustment
		in *= 0.5;
		
		sourceMicDelay->write(in);
		
		updateConnectionLengths();
		
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
		source.setX(x);
		source.setY(y);
		source.setZ(z);
		
		recalculateConnectionLengths();
	}
	
	void Network::setMicPosition(float x, float y, float z) {
		mic.setX(x);
		mic.setY(y);
		mic.setZ(z);
		
		recalculateConnectionLengths();
	}
	
	void Network::recalculateConnectionLengths()
	{
		for(int node = 0; node < nodeCount; node++)
		{
			nodes[node].setPosition(bounds[node].getScatteringNodePosition(mic, source));
		}
	}
	
	void Network::updateConnectionLengths()
	{
		int connection = 0;
		for(int node = 0; node < nodeCount - 1; node++)
		{
			for(int otherNode = node + 1; otherNode < nodeCount; otherNode++)
			{
				connections[connection].setLength(nodes[node].getPosition().distanceTo(nodes[otherNode].getPosition()));
				connection++;
			}
		}
		
		for(int node = 0; node < nodeCount; node++)
		{
			sourceToNodeDelays[node].setDelayLengthFromDistance(source.distanceTo(nodes[node].getPosition()));
			nodeToMicDelays[node].setDelayLengthFromDistance(mic.distanceTo(nodes[node].getPosition()));
		}
		
		sourceMicDelay->setDelayLengthFromDistance(source.distanceTo(mic));
		
		
	}
}
