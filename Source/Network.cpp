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
		// define wall positions
		bounds[0] = SDN::Boundary(0.0, SDN::Plane::YZ);
		bounds[1] = SDN::Boundary(width, SDN::Plane::YZ);
		bounds[2] = SDN::Boundary(0.0, SDN::Plane::XZ);
		bounds[3] = SDN::Boundary(length, SDN::Plane::XZ);
		bounds[4] = SDN::Boundary(0.0, SDN::Plane::XY);
		bounds[5] = SDN::Boundary(height, SDN::Plane::XY);
		
		// calculate number of connections and initialise node positions
		for(int node = 0; node < nodeCount; node++)
		{
			connectionCount += node;
			nodes[node] = SDN::Node(bounds[node].getScatteringNodePosition(mic, source), delayOrder);
		}
		
		connections = new SDN::Connection[connectionCount];
		
		// initialise connections between nodes, defining lengths and providing terminals to the nodes to interact with
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
		
		// initialised the source to node and node to mic delay lines
		for(int node = 0; node < nodeCount; node++)
		{
			sourceToNodeDelays[node] = *ModulatingDelay::fromDistance(sampleRate, source.distanceTo(nodes[node].getPosition()));
			nodeToMicDelays[node] = *ModulatingDelay::fromDistance(sampleRate, mic.distanceTo(nodes[node].getPosition()));
		}
				
		sourceMicDelay = Delay::fromDistance(sampleRate, source.distanceTo(mic));
	}
	
	// returns a stereo output of the direct line between the source and mic, based on their relative positions
	StereoOutput Network::positionSource(float sourceInput)
	{
		sourceInput *= 0.2; // accommodate 5x gain factor in 1/r volume adjustment
		sourceMicDelay->write(sourceInput);
		
		StereoOutput out;
		
		float fromSource = sourceMicDelay->read() / source.distanceTo(mic); // get value from delay line and attenuate by 1/r
		float sourceAzimuth = source.azimuthFrom(mic);
		float sinAzimuth = sin(sourceAzimuth);
		float denom = sqrt(2 * (1 + pow(sinAzimuth, 2)));
		float sourceGainLeft = (1 - sinAzimuth) / denom;
		float sourceGainRight = (1 + sinAzimuth) / denom;
		
		out.L = fromSource * sourceGainLeft;
		out.R = fromSource * sourceGainRight;
		
		return out;
	}
	
	// returns a stereo signal of a the reverberation.
	StereoOutput Network::scatterStereo(float in)
	{
		scatter(in);
		
		StereoOutput out;
		out.L = 0.0;
		out.R = 0.0;
		
		for(int node = 0; node < nodeCount; node++)
		{
			float fromNode = nodeToMicDelays[node].read();
			fromNode /= (mic.distanceTo(nodes[node].getPosition())); // get value from delay line and attenuate by 1/r
			float nodeAzimuth = nodes[node].getPosition().azimuthFrom(mic);
			float sinAzimuth = sin(nodeAzimuth);
			float denom = 1 / sqrt(2 * (1 + pow(sinAzimuth, 2)));
			
			float nodeGainLeft = (1 - sinAzimuth) * denom; // calculate left gain
			float nodeGainRight = (1 + sinAzimuth) * denom; // calculate right gain
			
			out.L += fromNode * nodeGainLeft;
			out.R += fromNode * nodeGainRight;
		}
		
		return out;
	}
	
	
	// method for scattering only in mono
	float Network::scatterMono(float in)
	{
		scatter(in);
		auto out = sourceMicDelay->read()/source.distanceTo(mic); // get value from delay line and attenuate by 1/r
		
		for(int node = 0; node < nodeCount; node++)
		{
			float micDistance = mic.distanceTo(nodes[node].getPosition());
			float sourceDistance = source.distanceTo(nodes[node].getPosition());
			float nodeReading = nodeToMicDelays[node].read();
			float nodeOut = nodeReading / (1 + (micDistance / sourceDistance));
			out += nodeOut;
		}
		return out;
	}
	
	// main reverberation method
	void Network::scatter(float in)
	{
		in *= 0.2; // accommodate 5x gain factor in 1/r volume adjustment
		
		// for each node, gather inputs
		for(int node = 0; node < nodeCount; node++)
		{
			sourceToNodeDelays[node].write(in);
			nodes[node].gatherInputWaveVectorFromNodes();
		}
		
		// scatter and distribute
		for(int node = 0; node < nodeCount; node++)
		{
			float nodeOut = nodes[node].getNodeOutput();
			nodes[node].scatter(sourceToNodeDelays[node].read() / source.distanceTo(nodes[node].getPosition())); // add distance attenuation
			nodes[node].distributeOutputWaveVectorToNodes();
			nodeToMicDelays[node].write(nodeOut);
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
	
	
	// update lengths if mic or source position have changed
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
		
		for(int node = 0; node < nodeCount; node++)
		{
			sourceToNodeDelays[node].setDelayLengthFromDistance(source.distanceTo(nodes[node].getPosition()));
			nodeToMicDelays[node].setDelayLengthFromDistance(mic.distanceTo(nodes[node].getPosition()));
		}
		
		sourceMicDelay->setDelayLengthFromDistance(source.distanceTo(mic));
	}
	
	void Network::setAbsorptionAmount(const float amount) {
		for(int node = 0; node < nodeCount; node++)
		{
			nodes[node].setAbsorption(amount);
		}
	}
}
