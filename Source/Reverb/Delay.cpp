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

inline float getAirAttenuationCoeffFromDistance(float distance) {
	return 0.2 * log((distance/3) + 1);
}

namespace SDN {
	Delay::Delay(float sampleRate, int delayInSamples) :
	bufferLength(4 * delayInSamples),
	sampleRate(sampleRate),
	distance(SDN::c * delayInSamples / sampleRate)
	{
		
		buffer = new float[bufferLength];
		memset(buffer, 0.0, bufferLength * sizeof(float));
		writePointer = 0;
		
		readPointer = (writePointer - delayInSamples);
		readPointer += bufferLength;
		if (readPointer >= bufferLength)
			readPointer -= bufferLength;
		
		airAbsorptionFilter.prepare(1);
		setAirAbsorption();
	}
	
	Delay* Delay::fromDistance(float sampleRate, float distance)
	{
		return new Delay(sampleRate, (sampleRate * distance / SDN::c));
	}
	
	void Delay::setAirAbsorption(){
		airAbsorptionCoefficient = getAirAttenuationCoeffFromDistance(distance);
		float a[] = {1.0, -airAbsorptionCoefficient};
		float b[] = {1 - airAbsorptionCoefficient, 0.0};
		airAbsorptionFilter.setCoefficients(a, b);
	}
	
	// for updating length
	void Delay::setDelayLength(int delayInSamples)
	{
		readPointer = writePointer - delayInSamples;
		readPointer += bufferLength;
		if (readPointer >= bufferLength)
			readPointer -= bufferLength;
	}
	
	void Delay::setDelayLengthFromDistance(float d)
	{
		setDelayLength(sampleRate * d / SDN::c);
		distance = d;
		setAirAbsorption();
	}
	
	float Delay::getDelayDistance() {
		return distance;
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
		float out = 0.0;
		
//		if(fractional) {
			// get high sample for linearly interpolation
			int highPointer = floor(readPointer + 1.0);
			if(highPointer >= bufferLength) highPointer -= bufferLength;
			
			float high = buffer[highPointer];
			float low = buffer[(int) floor(readPointer)];
			out = (1 - (readPointer - floor(readPointer))) * low + (readPointer - floor(readPointer)) * high; // interpolate
			
			incrementReadPointer();
			
			if(bufferLength < 0)
				readPointer += bufferLength;
			else if (readPointer >= bufferLength)
				readPointer -= bufferLength;
//		} else {
//			out = buffer[readPointer];
//			readPointer++;
//
//			readPointer = (readPointer + bufferLength) % bufferLength;
//		}
		
		return out;
	}
	
	float Delay::readWithAirAbsorption(){
		return airAbsorptionFilter.processSample(read());
	}
	
	float Delay::readWithDistanceAttenuation(float adjustment)
	{
		return readWithAirAbsorption()/(distance + 1.0 + adjustment);
	}

}
