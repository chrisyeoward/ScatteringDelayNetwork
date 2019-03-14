/*
  ==============================================================================

    Node.h
    Created: 11 Mar 2019 10:23:28pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once
#include "Point.h"
#include <string.h>

namespace SDN {
    class Node {
		private:
			Point position;
			float* scatteringMatrix;
			int delayOrder = 2;
		
		public:
			void scatter(float sourceInput, float* waveVector);
			Point getPosition();
			Node(Point position);
			~Node() {}
		
	};
}
