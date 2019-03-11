/*
  ==============================================================================

    Point.cpp
    Created: 11 Mar 2019 8:57:08pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Point.h"

namespace SDN {
	Point::Point(int x, int y) : x(x), y(y) {}
	
	float Point::distanceTo(Point point)
	{
		return sqrt(pow(point.x - x,2) + pow(point.y - y,2));
	}
}
