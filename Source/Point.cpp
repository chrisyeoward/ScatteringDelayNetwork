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
		return sqrt(pow(point.getX() - x,2) + pow(point.getY() - y,2) + pow(point.getZ() - z,2));
	}
	
	float Point::azimuthFrom(Point point)
	{
		float dx = x - point.getX();
		float dy = abs(y - point.getY());
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
