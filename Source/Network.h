/*
  ==============================================================================

    ScatteringDelay.h
    Created: 11 Mar 2019 6:12:29pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once

#include "Delay.h"
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
	
		SDN::Boundary bounds[nodeCount] = { SDN::Boundary(0.0, SDN::Plane::YZ),
			SDN::Boundary(5.0, SDN::Plane::YZ),
			SDN::Boundary(0.0, SDN::Plane::XZ),
			SDN::Boundary(5.0, SDN::Plane::XZ),
			SDN::Boundary(0.0, SDN::Plane::XY),
			SDN::Boundary(3.0, SDN::Plane::XY)
		};
		
		SDN::Node nodes[nodeCount];
		
		SDN::Connection* connections;
		
		SDN::Delay sourceToNodeDelays[nodeCount];
		SDN::Delay nodeToMicDelays[nodeCount];
		
		SDN::Delay *sourceMicDelay;
		
		void scatter(float input);
		
		void updateConnectionLengths();
		void recalculateConnectionLengths();
		
		public:
		SDN::StereoOutput scatterStereo(float input);
		float scatterMono(float input);
	
		void setSourcePosition(float x, float y, float z);
		void setMicPosition(float x, float y, float z);
		
		Network(float sampleRate);
		~Network();
	};
}

