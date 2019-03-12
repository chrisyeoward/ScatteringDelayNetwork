/*
  ==============================================================================

    Node.cpp
    Created: 11 Mar 2019 10:23:28pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Node.h"

namespace SDN {
	Node::Node(Point position) : position(position)
	{}
	
	Point Node::getPosition()
	{
		return position;
	}
}
