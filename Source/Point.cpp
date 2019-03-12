/*
  ==============================================================================

    Point.cpp
    Created: 11 Mar 2019 8:57:08pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Point.h"

namespace SDN {
	Point::Point(float x, float y) : x(x), y(y), z(0.0) {}
	
	float Point::distanceTo(Point point)
	{
		return sqrt(pow(point.x - x,2) + pow(point.y - y,2));
	}
	
	float Point::getX(){
		return x;
	}
	
	float Point::getY(){
		return y;
	}
}
