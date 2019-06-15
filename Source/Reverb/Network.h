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

/*
 This class does all the orchestration between the components
 It defines a source and mic position, wall boundaries and reflection coefficients.
 It manages positions and updates accordingly.
 It determines the stereo output based on the locations of the source and walls.
 */

namespace SDN
{
	class Network {
		private:
		
		const static int nodeCount = 6;
		const int delayOrder = nodeCount - 1;
		int connectionCount = 0;
	
		SDN::Point source = SDN::Point(2.5, 4.0, 1.5); // default source position
		SDN::Point mic = SDN::Point(2.5, 0.5, 1.8); // default source position
	
		SDN::Boundary bounds[nodeCount]; // walls
		
		SDN::Node nodes[nodeCount]; // scattering junction nodes
		
		SDN::Connection* connections; // inter node connections
		
		SDN::ModulatingDelay sourceToNodeDelays[nodeCount];
		SDN::ModulatingDelay nodeToMicDelays[nodeCount];
		
		SDN::ModulatingDelay *sourceMicDelay; // direct delay line
		
		void scatter(float input); // function to do the scattering
		
		void updateNodePositions(); // updates the connection lengths based on a new source or mic position
		
		float azimuthFor(Point point);
		
		public:
		SDN::StereoOutput scatterStereo(float input);
		float scatterMono(float input);
		SDN::StereoOutput positionSource(float sourceInput);
		
		void process(float input, float* output);
		
		void getNodeElevations(int* elevations);
		float getNodeElevation(int node);
		float getSourceElevation();
		
		void getNodeAzimuths(int *azimuths);
		float getNodeAzimuth(int node);
		float getSourceAzimuth();
		
		void setSourcePosition(float x, float y, float z);
		void setMicPosition(float x, float y, float z);
		
		void setAbsorptionAmount(const float amount);
		
		Network(float sampleRate);
		Network(float sampleRate, float width, float length, float height);
		~Network();
	};
}

