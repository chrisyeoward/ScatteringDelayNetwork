/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "XYContainer.h"

//==============================================================================
/**
*/
class ScatteringDelayReverbAudioProcessorEditor  : public AudioProcessorEditor, public Slider::Listener
{
public:
    ScatteringDelayReverbAudioProcessorEditor (ScatteringDelayReverbAudioProcessor&);
    ~ScatteringDelayReverbAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
	
	void sliderValueChanged (Slider*) override;

private:
	Slider absorptionSlider;
	Label absorptionLabel;
	Slider dryWetSlider;
	Label dryWetLabel;
		
	XYContainer roomContainer;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ScatteringDelayReverbAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScatteringDelayReverbAudioProcessorEditor)
};
