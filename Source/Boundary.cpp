/*
  ==============================================================================

    Wall.cpp
    Created: 11 Mar 2019 10:02:08pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Boundary.h"

namespace SDN {
	Boundary::Boundary(float position, Orientation orientation) : position(position), orientation(orientation){};
	
	Point Boundary::getScatteringNodePosition(Point mic, Point source){
		float h = source.getY() - mic.getY();
		float micDistance = position - mic.getX();
		float sourceDistance = position - source.getX();
		float offset = micDistance * h / (micDistance + sourceDistance);
		return Point(position, mic.getY() + offset);
	}
}
