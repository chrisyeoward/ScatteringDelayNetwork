/*
  ==============================================================================

    Point.h
    Created: 11 Mar 2019 8:57:08pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once
#include "math.h"

namespace SDN {
	class Point
	{
		private:
			float x, y, z;
		
		public:
		float getX();
		float getY();
		float distanceTo(Point point);
		float azimuthFrom(Point point);
		Point(float x, float y);
		~Point(){}
	};
}
