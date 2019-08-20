/*
  ==============================================================================

    Filter.cpp
    Created: 16 Feb 2019 6:50:23pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "Filter.h"

Filter::Filter() {}

void Filter::prepare(int filterOrder) {
	order = filterOrder;

	a = new float[order + 1];
	b = new float[order + 1];
	
	feedforward = new float[order];
	feedback = new float[order];
	
	memset(a, 0.0, (order + 1) * sizeof(float));
	memset(b, 0.0, (order + 1) * sizeof(float));
	memset(feedforward, 0.0, order * sizeof(float));
	memset(feedback, 0.0, order * sizeof(float));
}

float Filter::processSample(float input) {
//	assert(order > -1);
	float ff = b[0] * input;
	float fb = 0.0;

	for(int i = 0; i < order; i++) {
		ff += (b[i + 1] * feedforward[i]);
		fb += (a[i + 1] * feedback[i]);
	}

	float output = (ff - fb)/a[0];
	
	for(int i = order - 1; i > 0; i--) {
		feedforward[i] = feedforward[i - 1];
		feedback[i] = feedback[i - 1];
	}

	feedforward[0] = input;
	feedback[0] = output;

	return output;
}

void Filter::setCoefficients(float* newA, float* newB) {
//	assert(order > -1);
	for(int i = 0; i < order + 1; i++) {
		a[i] = newA[i];
		b[i] = newB[i];
	}
}
