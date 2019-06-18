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
	Delay::Delay(float sampleRate, int delayInSamples) :
	bufferLength(4 * delayInSamples),
	sampleRate(sampleRate),
	distance(SDN::c * delayInSamples / sampleRate)
	{
		buffer = new float[bufferLength];
		memset(buffer, 0.0, bufferLength * sizeof(float));
		writePointer = 0;
		
		readPointer = writePointer - delayInSamples;
		readPointer = (readPointer + bufferLength) % bufferLength;
	}
	
	Delay* Delay::fromDistance(float sampleRate, float distance)
	{
		return new Delay(sampleRate, (sampleRate * distance / SDN::c));
	}
	
	// for updating length
	void Delay::setDelayLength(int delayInSamples)
	{
		readPointer = writePointer - delayInSamples;
		readPointer = (readPointer + bufferLength) % bufferLength;
	}
	
	void Delay::setDelayLengthFromDistance(float d)
	{
		setDelayLength(sampleRate * d / SDN::c);
		distance = d;
	}
	
	float Delay::process(float sample) {
		write(sample);
		return read();
	}
	
	// writes to the buffer and increments
	void Delay::write(float sample) {
		buffer[writePointer] = sample;
		writePointer++;
		
		writePointer = (writePointer + bufferLength) % bufferLength;
	}
	
	void Delay::incrementReadPointer()
	{
		readPointer += 1.0;
	}
	
	// reads next sample
	float Delay::read() {
		float out = buffer[readPointer];
		readPointer++;
		
		readPointer = (readPointer + bufferLength) % bufferLength;
		return out;
	}
	
	float Delay::readWithDistanceAttenuation()
	{
		return read()/(distance + 1.0);
	}
}
