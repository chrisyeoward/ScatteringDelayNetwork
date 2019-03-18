/*
  ==============================================================================

    SmoothedFloat.cpp
    Created: 18 Mar 2019 2:01:57pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "SmoothedFloat.h"

namespace SDN {
	void SmoothedFloat::setValue(float value, bool forceUpdate)
	{
		if(true || forceUpdate) currentValue = value;
		targetValue = value;
		rampCount = 0;
		delta = (targetValue - currentValue) / rampSamples;
	}
	
	void SmoothedFloat::setValue(float value)
	{
		setValue(value, false);
	}
	
	float SmoothedFloat::getValue()
	{
		if (rampCount <= rampSamples) {
			rampCount++;
			isSmoothing = false;
			currentValue += delta;
		}
		return currentValue;
	}
}
