/*
  ==============================================================================

    SmoothedFloat.h
    Created: 18 Mar 2019 2:01:57pm
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once

namespace SDN 
{
    class SmoothedFloat {
		private:
		float currentValue;
		float targetValue;
		bool isSmoothing = false;
		
		int rampSamples = 10000;
		int rampCount = 0;
		float delta = 0;
		
		public:
		void setValue(float value);
		void setValue(float value, bool forceUpdate);
		float getValue();
		
		SmoothedFloat() {}
		SmoothedFloat(float initialValue) : currentValue(initialValue), targetValue(initialValue){}
	};
}
