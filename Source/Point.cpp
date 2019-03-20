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
		return fmax(0.1, sqrt(pow(point.getX() - getX(),2) + pow(point.getY() - getY(),2) + pow(point.getZ() - getZ(),2)));
	}
	
	float Point::azimuthFrom(Point point)
	{
		float dx = getX() - point.getX();
		float dy = abs(getY() - point.getY());
		return atan(dx/dy);
	}
	
	void Point::setX(float x){
		this->x.setValue(x);
	}
	
	float Point::getX(){
		return x.getValue();
	}
	
	void Point::setY(float y){
		this->y.setValue(y);
	}
	
	float Point::getY(){
		return y.getValue();
	}
	
	void Point::setZ(float z){
		this->z.setValue(z);
	}
	
	float Point::getZ(){
		return z.getValue();
	}	
}
