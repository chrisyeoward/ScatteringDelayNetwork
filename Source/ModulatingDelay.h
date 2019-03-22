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
#include "../JuceLibraryCode/JuceHeader.h"
#include "Constants.h"


namespace SDN 
{
	class ModulatingDelay : public SDN::Delay
    {
        private:
        float modFreq;
        float phase = 0.0;
		float amount = 0.2;
		float sampleRate;
        
        public:
		void incrementReadPointer();
		
		static ModulatingDelay* fromDistance(float sampleRate, float distance);
        
        ModulatingDelay(){};
		ModulatingDelay(float sampleRate, float delayInSamples);
        ~ModulatingDelay() {};
	};
}
