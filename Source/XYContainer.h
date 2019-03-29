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
 Container component for the XY Pad
 
 Manages the sources position, and updates the parameters based on change
 
*/
class XYContainer    : public Component, public Timer
{
public:
    XYContainer(ScatteringDelayReverbAudioProcessor&);
    ~XYContainer();

    void paint (Graphics&) override;
    void resized() override;
	void timerCallback() override;
	
	virtual void mouseDrag (const MouseEvent& event) override; // handle mousedrag

private:
	XYPoint source; 
	
	Label roomSizeLabel;
	
	ScatteringDelayReverbAudioProcessor& processor;
	
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XYContainer)
};
