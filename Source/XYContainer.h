/*
  ==============================================================================

    XYContainer.h
    Created: 18 Mar 2019 9:56:04am
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "XYPoint.h"
#include "PluginProcessor.h"


//==============================================================================
/*
*/
class XYContainer    : public Component, public Timer
{
public:
    XYContainer(ScatteringDelayReverbAudioProcessor&);
    ~XYContainer();

    void paint (Graphics&) override;
    void resized() override;
	void timerCallback() override;
	
	virtual void mouseDrag (const MouseEvent& event) override;

private:
	XYPoint source;
	
	ScatteringDelayReverbAudioProcessor& processor;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XYContainer)
};
