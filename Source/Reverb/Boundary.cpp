/*
  ==============================================================================

    Wall.cpp
    Created: 11 Mar 2019 10:02:08pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Boundary.h"

namespace SDN {
	Boundary::Boundary(float position, Plane orientation) : position(position), orientation(orientation){};
	
	Point Boundary::getScatteringNodePosition(Point mic, Point source){
		switch(orientation) {
			case Plane::XZ : {
				float parallelXDistance = source.getX() - mic.getX();
				float parallelZDistance = source.getZ() - mic.getZ();
				
				float micDistance = position - mic.getY();
				float sourceDistance = position - source.getY();
				
				float offsetX = micDistance * parallelXDistance / (micDistance + sourceDistance);
				float offsetZ = micDistance * parallelZDistance / (micDistance + sourceDistance);
				
				return Point(mic.getX() + offsetX, position, mic.getZ() + offsetZ);
				break;
			}
			case Plane::YZ : {
				float parallelYDistance = source.getY() - mic.getY();
				float parallelZDistance = source.getZ() - mic.getZ();
				
				float micDistance = position - mic.getX();
				float sourceDistance = position - source.getX();
				
				float offsetY = micDistance * parallelYDistance / (micDistance + sourceDistance);
				float offsetZ = micDistance * parallelZDistance / (micDistance + sourceDistance);
				
				return Point(position, mic.getY() + offsetY, mic.getZ() + offsetZ);
				break;
			}
			case Plane::XY : {
				float parallelYDistance = source.getY() - mic.getY();
				float parallelXDistance = source.getX() - mic.getX();
				
				float micDistance = position - mic.getZ();
				float sourceDistance = position - source.getZ();
				
				float offsetY = micDistance * parallelYDistance / (micDistance + sourceDistance);
				float offsetX = micDistance * parallelXDistance / (micDistance + sourceDistance);
				
				return Point(mic.getX() + offsetX, mic.getY() + offsetY, position);
				break;
			}
			default:
				return Point(0,0,0);
		}
	}
}
