/*
  ==============================================================================

    Wall.h
    Created: 11 Mar 2019 10:02:08pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once
#include "Node.h"
#include "Point.h"

namespace SDN {
	enum class Orientation {
		X,
		Y,
		Z
	};
	
    class Boundary 
    {
        private: 
        float position;
        Orientation orientation;
        
        public:
        
        Boundary(float position, Orientation orientation);
        ~Boundary() {}
		
		Point getScatteringNodePosition(Point mic, Point Source);
	};
}
