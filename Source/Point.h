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
			int x, y;
		
		public:
			float distanceTo(Point point);
			Point(int x, int y);
			~Point(){}
	};
}
