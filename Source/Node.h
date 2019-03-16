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

namespace SDN {
    class Node {
		private:
		Point position;
		float* scatteringMatrix;
		static const int delayOrder = 2;
		int terminalCount = 0;
		static const int maxTerminalCount = delayOrder;
		SDN::Terminal *terminals[maxTerminalCount];
		
		float waveVector[delayOrder];
		
		public:
		void gatherInputWaveVector();
		void distributeOutputWaveVector();
		float getNodeOutput();
		
		void addTerminal(SDN::Terminal *terminal);
		void scatter(float sourceInput);
		Point getPosition();
		Node(Point position);
		~Node() {}
		
	};
}
