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
		sourceMicDelay = new SDN::Delay(sampleRate, source.distanceTo(mic));
		
		sourceNode1Delay = new SDN::Delay(sampleRate, source.distanceTo(node1.getPosition()));
		sourceNode2Delay = new SDN::Delay(sampleRate, source.distanceTo(node2.getPosition()));
		sourceNode3Delay = new SDN::Delay(sampleRate, source.distanceTo(node3.getPosition()));
		//
		auto node1node2Distance = node1.getPosition().distanceTo(node2.getPosition());
		node1ToNode2Delay = new SDN::Delay(sampleRate, node1node2Distance);
		node2ToNode1Delay = new SDN::Delay(sampleRate, node1node2Distance);
		
		auto node1node3Distance = node1.getPosition().distanceTo(node3.getPosition());
		node1ToNode3Delay = new SDN::Delay(sampleRate, node1node3Distance);
		node3ToNode1Delay = new SDN::Delay(sampleRate, node1node3Distance);
		
		auto node3node2Distance = node3.getPosition().distanceTo(node2.getPosition());
		node3ToNode2Delay = new SDN::Delay(sampleRate, node3node2Distance);
		node2ToNode3Delay = new SDN::Delay(sampleRate, node3node2Distance);
		//
		node1MicDelay = new SDN::Delay(sampleRate, mic.distanceTo(node1.getPosition()));
		node2MicDelay = new SDN::Delay(sampleRate, mic.distanceTo(node2.getPosition()));
		node3MicDelay = new SDN::Delay(sampleRate, mic.distanceTo(node3.getPosition()));
	}
	
	float Network::scatter(float in)
	{
		in *= 0.5;
		
		sourceMicDelay->write(in);
		
		sourceNode1Delay->write(in);
		sourceNode2Delay->write(in);
		sourceNode3Delay->write(in);
		
		float node1WaveVector[] = {node2ToNode1Delay->read(), node3ToNode1Delay->read()};
		node1.scatter(sourceNode1Delay->read(), node1WaveVector);
		
		float node2WaveVector[] = {node1ToNode2Delay->read(), node3ToNode2Delay->read()};
		node2.scatter(sourceNode2Delay->read(), node2WaveVector);
		
		float node3WaveVector[] = {node1ToNode3Delay->read(), node2ToNode3Delay->read()};
		node2.scatter(sourceNode3Delay->read(), node3WaveVector);
		
		node1ToNode2Delay->write(node1WaveVector[0]);
		node1ToNode3Delay->write(node1WaveVector[1]);
		
		node2ToNode1Delay->write(node2WaveVector[0]);
		node2ToNode3Delay->write(node2WaveVector[1]);
		
		node3ToNode1Delay->write(node3WaveVector[0]);
		node3ToNode2Delay->write(node3WaveVector[1]);
		
		node1MicDelay->write(node1WaveVector[0] + node1WaveVector[1]);
		node2MicDelay->write(node2WaveVector[0] + node2WaveVector[1]);
		node3MicDelay->write(node3WaveVector[0] + node3WaveVector[1]);
		
		auto out = sourceMicDelay->read();
		out += node1MicDelay->read();
		out += node2MicDelay->read();
		out += node3MicDelay->read();
		
		return out;
	}
}
