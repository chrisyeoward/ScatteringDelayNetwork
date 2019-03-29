/*
  ==============================================================================

    Delay.cpp
    Created: 11 Mar 2019 6:38:45pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Delay.h"


// buffer length is arbitrarily chosen to be several times higher than delay length.
// no mechanism implemented for resizing the buffer dynamically, so a high enough value was chosen
// THIS BREAKS DOWN FOR VERY LARGE BUFFER SIZES

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
	
	Delay* Delay::fromDistance(float sampleRate, float distance)
	{
		return new Delay(sampleRate, (sampleRate * distance / SDN::c));
	}
	
	// for updating length
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
	
	// writes to the buffer and increments
	void Delay::write(float sample) {
		buffer[writePointer++] = sample;
		
		writePointer = (writePointer + bufferLength) % bufferLength;
	}
	
	void Delay::incrementReadPointer()
	{
		readPointer += 1.0;
	}
	
	// reads next sample
	float Delay::read() {
		// get high sample for linearly interpolation
		int highPointer = floor(readPointer + 1.5);
		if(highPointer >= bufferLength) highPointer -= bufferLength;

		float high = buffer[highPointer];
		float low = buffer[(int) floor(readPointer)];
		float out = (1 - (readPointer - floor(readPointer))) * low + (readPointer - floor(readPointer)) * high; // linearly interpolate

		incrementReadPointer();
		
		// housekeeping to keep within buffer
		if(bufferLength < 0)
			readPointer += bufferLength;
		else if (readPointer >= bufferLength)
			readPointer -= bufferLength;
		
		return out;
	}
}

