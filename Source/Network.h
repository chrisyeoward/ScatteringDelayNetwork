/*
  ==============================================================================

    ScatteringDelay.h
    Created: 11 Mar 2019 6:12:29pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once

#include "Delay.h"
#include "ModulatingDelay.h"
#include "Point.h"
#include "Boundary.h"
#include "Node.h"
#include "Connection.h"
#include "StereoOutput.h"
#include <math.h>
#include "../JuceLibraryCode/JuceHeader.h"

namespace SDN
{
	class Network {
		private:
		
		const static int nodeCount = 6;
		const int delayOrder = nodeCount - 1;
		int connectionCount = 0;
	
		SDN::Point source = SDN::Point(2.5, 4.0, 1.5);
		SDN::Point mic = SDN::Point(2.5, 0.5, 1.5);
	
		SDN::Boundary bounds[nodeCount];
		
		SDN::Node nodes[nodeCount];
		
		SDN::Connection* connections;
		
		SDN::ModulatingDelay sourceToNodeDelays[nodeCount];
		SDN::ModulatingDelay nodeToMicDelays[nodeCount];
		
		SDN::Delay *sourceMicDelay;
		
		void scatter(float input);
		
		void updateConnectionLengths();
		
		public:
		SDN::StereoOutput scatterStereo(float input);
		float scatterMono(float input);
		SDN::StereoOutput positionSource(float sourceInput);
	
		void setSourcePosition(float x, float y, float z);
		void setMicPosition(float x, float y, float z);
		
		void setAbsorptionAmount(const float amount);
		
		Network(float sampleRate);
		Network(float sampleRate, float width, float length, float height);
		~Network();
	};
}

