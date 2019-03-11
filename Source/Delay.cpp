/*
  ==============================================================================

    Delay.cpp
    Created: 11 Mar 2019 6:38:45pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Delay.h"

Delay::Delay(int d) :
	bufferLength(d),
	delayInSamples(d)
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
