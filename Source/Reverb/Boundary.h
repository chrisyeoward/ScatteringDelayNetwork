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

/*
 This represents a wall, its orientation and position.
 
 */

namespace SDN {
	enum class Plane {
		XZ, // perpendicular to Y
		XY, // perpendicular to Z
		YZ, // perpendicular to X
	};
	
    class Boundary 
    {
        private: 
        float position;
        Plane orientation; 
        
        public:
		
		Boundary() {};
        Boundary(float position, Plane orientation);
        ~Boundary() {}
		
		Point getScatteringNodePosition(Point mic, Point Source); // based on the mic and source position, get point of first reflection
	};
}
