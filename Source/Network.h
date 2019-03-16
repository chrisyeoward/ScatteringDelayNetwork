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

namespace SDN
{
	class Network {
		private:
		
			const static int nodeCount = 3;
			const int delayOrder = nodeCount - 1;
			const static int connectionCount = 3;
		
			SDN::Point source = SDN::Point(0.5, 0.0);
			SDN::Point mic = SDN::Point(1.2, 8.0);
		
			SDN::Boundary bounds[nodeCount] = { SDN::Boundary(0.0, SDN::Orientation::Y),
				SDN::Boundary(2.0, SDN::Orientation::Y),
				SDN::Boundary(10.0, SDN::Orientation::X) };
		
		SDN::Node nodes[nodeCount] = {SDN::Node(bounds[0].getScatteringNodePosition(mic, source)),
			SDN::Node(bounds[1].getScatteringNodePosition(mic, source)),
			SDN::Node(bounds[2].getScatteringNodePosition(mic, source))
		};
		
		SDN::Connection* connections;
		
		SDN::Delay sourceToNodeDelays[nodeCount];
		SDN::Delay nodeToMicDelays[nodeCount];
		
			SDN::Delay *sourceMicDelay;
		
//			SDN::Delay *sourceNode1Delay;
//			SDN::Delay *sourceNode2Delay;
//			SDN::Delay *sourceNode3Delay;
//		
//			SDN::Delay *node1MicDelay;
//			SDN::Delay *node2MicDelay;
//			SDN::Delay *node3MicDelay;
		
		public:
			float scatter(float input);
			
			Network(float sampleRate);
			~Network();
	};
}

