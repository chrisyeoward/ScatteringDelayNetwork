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

namespace SDN {
	class Delay
	{
		private:
			float* buffer;
			int bufferLength;
			int delayInSamples;
			int readPointer, writePointer;
			float sampleRate;

		public:
		void setDelayLengthFromDistance(float distance);
		void setDelayLength(int delayInSamples);
		
		float process(float inputSample);
		void write(float sample);
		float read();
		
		static Delay* fromDistance(float sampleRate, float distance);
		
		Delay() {};
		Delay(float sampleRate, int delayInSamples);
//		Delay(float sampleRate, float distance);
		~Delay() {}
	};
}

#endif /* DELAY_H_ */
