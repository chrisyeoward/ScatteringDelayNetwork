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
		switch(orientation) {
			case Orientation::X : {
				float parallelDistance = source.getX() - mic.getX();
				float micDistance = position - mic.getY();
				float sourceDistance = position - source.getY();
				float offset = micDistance * parallelDistance / (micDistance + sourceDistance);
				return Point(mic.getX() + offset, position);
				break;
			}
			case Orientation::Y : {
				float parallelDistance = source.getY() - mic.getY();
				float micDistance = position - mic.getX();
				float sourceDistance = position - source.getX();
				float offset = micDistance * parallelDistance / (micDistance + sourceDistance);
				return Point(position, mic.getY() + offset);
				break;
			}
			case Orientation::Z : {
				return Point(0,0);
				break;
			}
			default:
				return Point(0,0);
		}
	}
}
