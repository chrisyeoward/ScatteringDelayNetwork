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

namespace SDN
{
	class Network {
		private:
			SDN::Point source = SDN::Point(0.5, 0.0);
			SDN::Point mic = SDN::Point(1.2, 8.0);
		
			SDN::Boundary bound1 = SDN::Boundary(0.0, SDN::Orientation::Y);
			SDN::Node node1 = SDN::Node(bound1.getScatteringNodePosition(mic, source));
		
			SDN::Boundary bound2 = SDN::Boundary(2.0, SDN::Orientation::Y);
			SDN::Node node2 = SDN::Node(bound2.getScatteringNodePosition(mic, source));
		
			SDN::Boundary bound3 = SDN::Boundary(10.0, SDN::Orientation::X);
			SDN::Node node3 = SDN::Node(bound3.getScatteringNodePosition(mic, source));
		
			SDN::Delay *sourceMicDelay;
		
			SDN::Delay *sourceNode1Delay;
			SDN::Delay *sourceNode2Delay;
			SDN::Delay *sourceNode3Delay;
		
			SDN::Delay *node1ToNode2Delay;
			SDN::Delay *node2ToNode1Delay;
		
			SDN::Delay *node1ToNode3Delay;
			SDN::Delay *node3ToNode1Delay;
		
			SDN::Delay *node3ToNode2Delay;
			SDN::Delay *node2ToNode3Delay;
		
			SDN::Delay *node1MicDelay;
			SDN::Delay *node2MicDelay;
			SDN::Delay *node3MicDelay;
		
		public:
			float scatter(float input);
			
			Network(float sampleRate);
			~Network();
	};
}

