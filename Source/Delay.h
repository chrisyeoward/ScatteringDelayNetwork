/*
  ==============================================================================

    Delay.h
    Created: 11 Mar 2019 6:38:45pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once

#ifndef DELAY_H_
#include <string.h>

class Delay
{
    
	//private difference eq member properties
    private:
	    float* buffer;
	    int bufferLength;
	    int delayInSamples;
	    int readPointer, writePointer;
	

	public:
        float process(float inputSample);
	
		Delay(int d);
        virtual ~Delay() {}
};

#endif /* DELAY_H_ */
