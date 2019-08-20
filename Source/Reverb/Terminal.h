/*
  ==============================================================================

    Terminal.h
    Created: 15 Mar 2019 5:00:32pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once
#include "Delay.h"

/*
 This class represents one end to a bidirectional delay line, with pointers to which delay to read and write to.
 Each node has as one of these for each other node to read and write to.

*/

namespace SDN {
    class Terminal
    {
		private:
        	SDN::Delay* readDelay;
        	SDN::Delay* writeDelay;
		public:
			void write(float sample);
			float read();
		
		Terminal() {};
		Terminal(SDN::Delay* readDelay, SDN::Delay* writeDelay);
		~Terminal() {
			
		};
	};
}
