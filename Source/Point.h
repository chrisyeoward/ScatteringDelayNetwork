/*
  ==============================================================================

    Point.h
    Created: 11 Mar 2019 8:57:08pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once
#include "math.h"
#include <iostream>

namespace SDN {
	class Point
	{
		private:
			float x, y, z;
		
		public:
		float getX();
		void setX(float x);
		
		float getY();
		void setY(float y);
		
		float getZ();
		void setZ(float z);
		
		float distanceTo(Point point);
		float azimuthFrom(Point point);
		
		Point() {};
		Point(float x, float y);
		Point(float x, float y, float z);
		~Point() {};
	};
}
