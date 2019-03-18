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
	enum class Plane {
		XZ,
		XY,
		YZ,
	};
	
    class Boundary 
    {
        private: 
        float position;
        Plane orientation;
        
        public:
        
        Boundary(float position, Plane orientation);
        ~Boundary() {}
		
		Point getScatteringNodePosition(Point mic, Point Source);
	};
}
