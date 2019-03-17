/*
  ==============================================================================

    Connection.h
    Created: 15 Mar 2019 4:10:21pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once
#include "Delay.h"
#include "Terminal.h"
#include "Node.h"
#include "Constants.h"

namespace SDN {
    class Connection 
    {
        private:
            SDN::Delay* startToEndDelay;
            SDN::Delay* endToStartDelay;
		
			SDN::Terminal* startTerminal;
			SDN::Terminal* endTerminal;
		
			float sampleRate;
		
		public:
		
		SDN::Terminal* getStartTerminal();
		SDN::Terminal* getEndTerminal();
		
		void setLength(float distance);
		
		Connection() {};
		Connection(float distance, float sampleRate);
		Connection(SDN::Node startNode, SDN::Node endNode, float sampleRate);
		~Connection() {};
	};
}

// connect node to node
// assign node to connection
