/*
  ==============================================================================

    Delay.h
    Created: 11 Mar 2019 6:38:45pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once

#ifndef DELAY_H_
#include <math.h>
#include <string.h>
#include "Constants.h"
#include "Filter.h"

/*
 Delay line class, with fractional read pointer
 */

namespace SDN {
	class Delay
	{
		protected:
		float* buffer;
		int bufferLength;
		float readPointer;
		int writePointer;
		float sampleRate;
		float distance;
		
		Filter airAbsorptionFilter;
		float airAbsorptionCoefficient;
		
		virtual void incrementReadPointer(); // overriden in modulating delay

		public:
		void setDelayLengthFromDistance(float distance);
		void setDelayLength(int delayInSamples);
		
		void setAirAbsorption();
		
		float getDelayDistance();
		
		float process(float inputSample);
		void write(float sample);
		float read();
		float readWithDistanceAttenuation(float adjustment = 0.0);
		float readWithAirAbsorption();
		
		static Delay* fromDistance(float sampleRate, float distance);
		
		Delay() {};
		Delay(float sampleRate, int delayInSamples);
//		Delay(float sampleRate, float distance);
		virtual ~Delay() {}
	};
}

#endif /* DELAY_H_ */
