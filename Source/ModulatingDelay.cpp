/*
  ==============================================================================

    ModulatingDelay.cpp
    Created: 21 Mar 2019 10:06:30pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "ModulatingDelay.h"

namespace SDN
{
	ModulatingDelay::ModulatingDelay(float sampleRate, float delayInSamples) : Delay(sampleRate, delayInSamples)
	{
		modFreq = (float) rand() / RAND_MAX;
		modFreq *= 5;
	}
	
	void ModulatingDelay::incrementReadPointer()
	{
		readPointer += 1.0;
		readPointer += amount * sin(phase);
		phase += 2.0 * M_PI * (modFreq/sampleRate);
		if(phase > 2.0 * M_PI)
			phase -= 2.0 * M_PI;
	}
	
	ModulatingDelay* ModulatingDelay::fromDistance(float sampleRate, float distance)
	{
		return new ModulatingDelay(sampleRate, (sampleRate * distance / SDN::c));
	}
}
