/*
  ==============================================================================

    Connection.h
    Created: 15 Mar 2019 4:10:21pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once
#include "ModulatingDelay.h"
#include "Terminal.h"
#include "Node.h"
#include "Constants.h"

/*
 A connection represents a birectional delay line between 2 nodes. It provides terminals at its start
 and end to which to interact with.
 */

namespace SDN {
    class Connection 
    {
        private:
            SDN::ModulatingDelay* startToEndDelay;
            SDN::ModulatingDelay* endToStartDelay;
		
			SDN::Terminal* startTerminal;
			SDN::Terminal* endTerminal;
		
			float sampleRate;
		
		public:
		
		SDN::Terminal* getStartTerminal();
		SDN::Terminal* getEndTerminal();
		
		void setLength(float distance); // update the length of the delay lines
		
		Connection() {};
		Connection(float distance, float sampleRate);
		Connection(SDN::Node startNode, SDN::Node endNode, float sampleRate);
		~Connection() {};
	};
}

// connect node to node
// assign node to connection
