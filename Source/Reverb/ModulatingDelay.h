/*
  ==============================================================================

    ModulatingDelay.h
    Created: 21 Mar 2019 10:06:30pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once
#include "Delay.h"
#include <cstdlib>
#include "Constants.h"
#include <iostream>

/*
 Modulated Delay line class. Read pointer fluctuates at random frequency 
 */


namespace SDN 
{
	class ModulatingDelay : public SDN::Delay
    {
        private:
        float modFreq;
        float phase = 0.0;
		float amount = 0.003;
		
        public:
		void incrementReadPointer() override;
				
		static ModulatingDelay* fromDistance(float sampleRate, float distance, float maxDistance);
        
        ModulatingDelay(){};
		ModulatingDelay(float sampleRate, float delayInSamples, int maxBuffer);
        ~ModulatingDelay() {};
	};
}
