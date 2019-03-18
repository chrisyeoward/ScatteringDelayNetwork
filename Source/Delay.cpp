/*
  ==============================================================================

    Delay.cpp
    Created: 11 Mar 2019 6:38:45pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Delay.h"

namespace SDN {
	Delay::Delay(float sampleRate, int delayInSamples) :
	bufferLength(5 * delayInSamples),
	delayInSamples(delayInSamples),
	sampleRate(sampleRate)
	{
		buffer = new float[bufferLength];
		memset(buffer, 0.0, bufferLength * sizeof(float));
		writePointer = 0;
		readPointer = (writePointer - delayInSamples + bufferLength) % bufferLength;
	}
	
	Delay::Delay(float sampleRate, float distance) : Delay(sampleRate, (int) floor((sampleRate * distance) / SDN::c)) {}
	
	Delay* Delay::fromDistance(float sampleRate, float distance)
	{
		return new Delay(sampleRate, (int) floor((sampleRate * distance) / SDN::c));
	}
	
	void Delay::setDelayLength(int delayInSamples)
	{
		readPointer = (writePointer - delayInSamples + bufferLength) % bufferLength;
	}
	
	void Delay::setDelayLengthFromDistance(float distance)
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

