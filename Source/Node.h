/*
  ==============================================================================

    Node.h
    Created: 11 Mar 2019 10:23:28pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once
#include "Point.h"
#include "Terminal.h"
#include <string.h>
#include "Constants.h"

namespace SDN {
    class Node {
		private:
		Point position;
		float* scatteringMatrix;
		int numberOfOtherNodes;
		int terminalCount = 0;
		static const int maxTerminalCount = MAXIMUM_NODES - 1;
		SDN::Terminal *terminals[maxTerminalCount];
		
		float* waveVector;
		
		public:
		void gatherInputWaveVectorFromNodes();
		void distributeOutputWaveVectorToNodes();
		float getNodeOutput();
		
		void addTerminal(SDN::Terminal *terminal);
		void scatter(float sourceInput);
		Point getPosition();
		void setPosition(Point p);
		
		Node () {};
		Node(Point position, int numberOfOtherNodes);
		~Node() {};
		
	};
}
