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
	Network::Network(float sampleRate) : Network(sampleRate, 5.0, 5.0, 3.0) {}
	
	Network::Network(float sampleRate, float width, float length, float height)
	{
		bounds[0] = SDN::Boundary(0.0, SDN::Plane::YZ);
		bounds[1] = SDN::Boundary(width, SDN::Plane::YZ);
		bounds[2] = SDN::Boundary(0.0, SDN::Plane::XZ);
		bounds[3] = SDN::Boundary(length, SDN::Plane::XZ);
		bounds[4] = SDN::Boundary(0.0, SDN::Plane::XY);
		bounds[5] = SDN::Boundary(height, SDN::Plane::XY);
		
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
		float denom = sqrt(2 * (1 + pow(sinAzimuth, 2)));
		float sourceGainLeft = (1 - sinAzimuth) / denom;
		float sourceGainRight = (1 + sinAzimuth) / denom;
		
		out.L = fromSource * sourceGainLeft;
		out.R = fromSource * sourceGainRight;
		
		if(abs(fromSource) >= 1) {
			DBG("Source peak");
			DBG(fromSource);
		}
		
		for(int node = 0; node < nodeCount; node++)
		{
			float fromNode = nodeToMicDelays[node].read();
			fromNode /= ((fmax(mic.distanceTo(nodes[node].getPosition()), 0.1) + (fmax(source.distanceTo(nodes[node].getPosition()), 0.1))));
			float nodeAzimuth = nodes[node].getPosition().azimuthFrom(mic);
			float sinAzimuth = sin(nodeAzimuth);
			float denom = 1 / sqrt(2 * (1 + pow(sinAzimuth, 2)));
			float nodeGainLeft = (1 - sinAzimuth) * denom;
			float nodeGainRight = (1 + sinAzimuth) * denom;
			
			out.L += fromNode * nodeGainLeft;
			out.R += fromNode * nodeGainRight;
			if(abs(fromNode) >= 1) {
				DBG("Node peak");
				DBG(fromNode);
			}
		}
		
		if(abs(out.L) >= 1 || abs(out.R) >= 1) {
			DBG("peaking in network");
		}
		
			
		return out;
	}
	
	float Network::scatterMono(float in)
	{
		scatter(in); // accommodate 10x gain factor in 1/r volume adjustment
		auto out = sourceMicDelay->read()/source.distanceTo(mic);
		
		if(out >= 1) {
			DBG("Source peak");
			DBG(out);
		}
		for(int node = 0; node < nodeCount; node++)
		{
			float micDistance = mic.distanceTo(nodes[node].getPosition());
			float sourceDistance = source.distanceTo(nodes[node].getPosition());
			float nodeReading = nodeToMicDelays[node].read();
			float nodeOut = nodeReading / (1 + (micDistance / sourceDistance));
			out += nodeOut;
			if(abs(nodeOut) >= 1) {
				DBG("\n");
				DBG("--------");
				DBG("Mic distance ");
				DBG(micDistance);
				DBG("Source distance");
				DBG(sourceDistance);
				DBG("node reading");
				DBG(nodeReading);
				DBG("Node peak");
				DBG(nodeOut);
				DBG("Out peak");
				DBG(out);
			}
		}
		return out;
	}
	
	void Network::scatter(float in)
	{
		in *= 0.1; // accommodate 10x gain factor in 1/r volume adjustment
		in *= 0.5;
		
		sourceMicDelay->write(in);
		
		for(int node = 0; node < nodeCount; node++)
		{
			sourceToNodeDelays[node].write(in);
			nodes[node].gatherInputWaveVectorFromNodes();
		}
		
		for(int node = 0; node < nodeCount; node++)
		{
			float nodeOut = nodes[node].getNodeOutput();
			nodes[node].scatter(sourceToNodeDelays[node].read() / (fmax(source.distanceTo(nodes[node].getPosition()), 0.1))); // add distance attenuation
			nodes[node].distributeOutputWaveVectorToNodes();
			nodeToMicDelays[node].write(nodeOut);
			if(abs(nodeOut) >= 1) {
				DBG("--------");
				DBG("Node writing");
				DBG(nodeOut);
				DBG("\n");
			}
		}
	}
	
	void Network::setSourcePosition(float x, float y, float z) {
		source.setX(x);
		source.setY(y);
		source.setZ(z);
		
		updateConnectionLengths();
	}
	
	void Network::setMicPosition(float x, float y, float z) {
		mic.setX(x);
		mic.setY(y);
		mic.setZ(z);
		
		updateConnectionLengths();
	}
	
	void Network::updateConnectionLengths()
	{
		for(int node = 0; node < nodeCount; node++)
		{
			nodes[node].setPosition(bounds[node].getScatteringNodePosition(mic, source));
		}
		
		int connection = 0;
		for(int node = 0; node < nodeCount - 1; node++)
		{
			for(int otherNode = node + 1; otherNode < nodeCount; otherNode++)
			{
				connections[connection].setLength(nodes[node].getPosition().distanceTo(nodes[otherNode].getPosition()));
				connection++;
			}
		}
		
//		DBG("Setting node positions....");
		for(int node = 0; node < nodeCount; node++)
		{
			sourceToNodeDelays[node].setDelayLengthFromDistance(source.distanceTo(nodes[node].getPosition()));
			nodeToMicDelays[node].setDelayLengthFromDistance(mic.distanceTo(nodes[node].getPosition()));
//			DBG("-------------");
//			DBG(nodes[node].getPosition().getX());
//			DBG(nodes[node].getPosition().getY());
//			DBG(nodes[node].getPosition().getZ());
//			DBG("\n");
		}
		
		sourceMicDelay->setDelayLengthFromDistance(source.distanceTo(mic));
	}
}
