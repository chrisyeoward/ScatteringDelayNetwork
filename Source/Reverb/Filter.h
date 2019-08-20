/*
  ==============================================================================

    Filter.h
    Created: 16 Feb 2019 6:46:15pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once


#ifndef FILTER_H_

#define FILTER_H_
//#define FILTER_ORDER 2

#define _USE_MATH_DEFINES

#include <cstring>

class Filter
{
    
	//private difference eq member properties
    protected:
	int order = -1;
	float* a;
	float* b;

	//private previous member properties describing previous input and outputs
	float* feedforward;
	float* feedback;

	public:
    /* processSample() applies the filter to the current input sample value,
	    * based on the values stored in the feedback and feedforward arrays.
	    *
	    * Calculates output using the general form of the 2nd order difference
	    * equation: a0y[n] = b0x[n] + b1x[n-1] + b2x[n-2] - (a1y[n-1] + a2y[n-2])
	    */
	float processSample(float input);
	
	void setCoefficients(float* a, float* b);
	
	void prepare(int order);
	
	Filter();
	~Filter() {
		delete[] a;
		delete[] b;
		delete[] feedforward;
		delete[] feedback;
	}
};

#endif /* FILTER_H_ */
