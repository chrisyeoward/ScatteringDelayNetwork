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

namespace SDN
{
	class Network {
		private:
		
		const static int nodeCount = 4;
		const int delayOrder = nodeCount - 1;
		int connectionCount = 0;
	
		SDN::Point source = SDN::Point(1.5, 4.0);
		SDN::Point mic = SDN::Point(2.5, 1.0);
	
		SDN::Boundary bounds[nodeCount] = { SDN::Boundary(0.0, SDN::Orientation::Y),
			SDN::Boundary(5.0, SDN::Orientation::Y),
			SDN::Boundary(5.0, SDN::Orientation::X),
			SDN::Boundary(0.0, SDN::Orientation::X)
		};
		
		SDN::Node nodes[nodeCount] = {SDN::Node(bounds[0].getScatteringNodePosition(mic, source), delayOrder),
			SDN::Node(bounds[1].getScatteringNodePosition(mic, source), delayOrder),
			SDN::Node(bounds[2].getScatteringNodePosition(mic, source), delayOrder),
			SDN::Node(bounds[3].getScatteringNodePosition(mic, source), delayOrder)
		};
		
		SDN::Connection* connections;
		
		SDN::Delay sourceToNodeDelays[nodeCount];
		SDN::Delay nodeToMicDelays[nodeCount];
		
		SDN::Delay *sourceMicDelay;
		
		void scatter(float input);
		
		public:
		SDN::StereoOutput scatterStereo(float input);
		float scatterMono(float input);
	
		
		Network(float sampleRate);
		~Network();
	};
}

