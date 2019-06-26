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
		modFreq = (float) rand() / RAND_MAX; // pick a random freq
		modFreq *= 3;
		std::cout << "frequency" << modFreq << "\n";
	}
	
	void ModulatingDelay::incrementReadPointer() // increment read pointer +1 with additional sine wave
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
