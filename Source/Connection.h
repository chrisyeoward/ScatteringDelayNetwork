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

namespace SDN {
    class Connection 
    {
        private:
            SDN::Delay* startToEndDelay;
            SDN::Delay* endToStartDelay;
		
			SDN::Terminal* startTerminal;
			SDN::Terminal* endTerminal;
		
			float distance;
		
		public:
		
		SDN::Terminal* getStartTerminal();
		SDN::Terminal* getEndTerminal();
		
		float readFrom(SDN::Delay *delay);
		void writeToStart(float sample);
		float readFromStart();
		
		void writeToEnd(float sample);
		float readFromEnd();
		
		Connection() {};
		Connection(float distance, float sampleRate);
		Connection(SDN::Node startNode, SDN::Node endNode, float sampleRate);
		~Connection() {};
	};
}

// connect node to node
// assign node to connection
