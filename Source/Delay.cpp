/*
  ==============================================================================

    Delay.cpp
    Created: 11 Mar 2019 6:38:45pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Delay.h"

namespace SDN {
	Delay::Delay(float sampleRate, float delayInSamples) :
	bufferLength(5 * (int) delayInSamples),
	sampleRate(sampleRate)
	{
		buffer = new float[bufferLength];
		memset(buffer, 0.0, bufferLength * sizeof(float));
		writePointer = 0;

		readPointer = (writePointer - delayInSamples);
		readPointer += bufferLength;
		if (readPointer >= bufferLength)
			readPointer -= bufferLength;
	}
	
//	Delay::Delay(float sampleRate, float distance) : Delay(sampleRate, (int) floor((sampleRate * distance) / SDN::c)) {}
	
	Delay* Delay::fromDistance(float sampleRate, float distance)
	{
		return new Delay(sampleRate, (sampleRate * distance / SDN::c));
	}
	
	void Delay::setDelayLength(float delayInSamples)
	{

		readPointer = (writePointer + bufferLength) - delayInSamples;
		if (readPointer >= bufferLength)
			readPointer -= bufferLength;
	}
	
	void Delay::setDelayLengthFromDistance(float distance)
	{
		setDelayLength(sampleRate * distance / SDN::c);
	}
	
	float Delay::process(float sample) {
		write(sample);
		return read();
	}
	
	void Delay::write(float sample) {
		buffer[writePointer++] = sample;
		
		writePointer = (writePointer + bufferLength) % bufferLength;
	}
	
	float Delay::read() {
		// get high sample for linearly interpolation
		int highPointer = floor(readPointer + 1.5);
		if(highPointer >= bufferLength) highPointer -= bufferLength;

		float high = buffer[highPointer];
		float low = buffer[(int) floor(readPointer)];
		float out = (1 - (readPointer - floor(readPointer))) * low + (readPointer - floor(readPointer)) * high; // interpolate

		readPointer += 1.0;
		
		if(bufferLength < 0)
			readPointer += bufferLength;
		else if (readPointer >= bufferLength)
			readPointer -= bufferLength;
		
		return out;
	}
}

