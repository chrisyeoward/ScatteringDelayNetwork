/*
  ==============================================================================

    Point.cpp
    Created: 11 Mar 2019 8:57:08pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Point.h"

namespace SDN {
	Point::Point(float x, float y) : Point(x,y,0) {}
	
	Point::Point(float x, float y, float z) : x(x), y(y), z(z) {}
	
	float Point::distanceTo(Point point)
	{
		return fmax(0.2, sqrt(pow(point.getX() - getX(),2) + pow(point.getY() - getY(),2) + pow(point.getZ() - getZ(),2)));
	}
	
	float Point::azimuthFrom(Point point)
	{
		float dx = getX() - point.getX();
		float dy = abs(getY() - point.getY()); // restrict y to be positive so angle within -90 and +90 degrees
		return atan(dx/dy);
	}
	
	void Point::setX(float x){
		this->x = x;
	}
	
	float Point::getX(){
		return x;
	}
	
	void Point::setY(float y){
		this->y = y;
	}
	
	float Point::getY(){
		return y;
	}
	
	void Point::setZ(float z){
		this->z = z;
	}
	
	float Point::getZ(){
		return z;
	}	
}
