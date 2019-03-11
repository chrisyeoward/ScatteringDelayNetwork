/*
  ==============================================================================

    Delay.cpp
    Created: 11 Mar 2019 6:38:45pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Delay.h"

namespace SDN {
	Delay::Delay(int delayInSamples) :
	bufferLength(delayInSamples),
	delayInSamples(delayInSamples)
	{
		buffer = new float[bufferLength];
		memset(buffer, 0.0, bufferLength * sizeof(float));
		writePointer = 0;
		readPointer = (writePointer - delayInSamples + bufferLength) % bufferLength;
	}
	
	float Delay::process(float sample) {
		float out = buffer[readPointer++];
		buffer[writePointer++] = sample;
		
		readPointer = (readPointer + bufferLength) % bufferLength;
		writePointer = (writePointer + bufferLength) % bufferLength;
		
		return out;
	}
}

