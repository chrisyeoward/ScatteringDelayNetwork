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
	bufferLength(4 * delayInSamples),
	delayInSamples(delayInSamples)
	{
		buffer = new float[bufferLength];
		memset(buffer, 0.0, bufferLength * sizeof(float));
		writePointer = 0;
		readPointer = (writePointer - delayInSamples + bufferLength) % bufferLength;
	}
	
	Delay::Delay(float sampleRate, float distance) : Delay(floor((sampleRate * distance) / SDN::c)) {}
	
	void Delay::setDelayLength(int delayInSamples)
	{
		readPointer = (writePointer - delayInSamples + bufferLength) % bufferLength;
	}
	
	void Delay::setDelayLengthFromDistance(float sampleRate, float distance)
	{
		setDelayLength(floor((sampleRate * distance) / SDN::c));
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
		float out = buffer[readPointer++];
		readPointer = (readPointer + bufferLength) % bufferLength;
		return out;
	}
}

