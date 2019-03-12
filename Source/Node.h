/*
  ==============================================================================

    Node.h
    Created: 11 Mar 2019 10:23:28pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once
#include "Point.h"

namespace SDN {
    class Node {
		private:
			Point position;
		
		public:
			Point getPosition();
			Node(Point position);
			~Node() {}
		
	};
}
