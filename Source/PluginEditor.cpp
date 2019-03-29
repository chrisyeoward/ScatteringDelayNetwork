/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ScatteringDelayReverbAudioProcessorEditor::ScatteringDelayReverbAudioProcessorEditor (ScatteringDelayReverbAudioProcessor& p)
    : AudioProcessorEditor (&p),
	absorptionSlider(Slider::RotaryVerticalDrag, Slider::TextBoxAbove),
	absorptionLabel("", "Wall Reflectivity Amount: "),
	dryWetSlider(Slider::RotaryVerticalDrag, Slider::TextBoxAbove),
	dryWetLabel("", "Dry/Wet Mix: "),
	roomContainer(p),
	processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	addAndMakeVisible(roomContainer);
	
	addAndMakeVisible (&absorptionSlider);
	absorptionSlider.addListener (this);
	absorptionSlider.setRange (0.0, 1.0, 0.001);
	absorptionSlider.setValue (processor.absorption->get());
	absorptionLabel.attachToComponent(&absorptionSlider, false);
	absorptionLabel.setJustificationType(Justification::centred);
	
	addAndMakeVisible (&dryWetSlider);
	dryWetSlider.addListener (this);
	dryWetSlider.setRange (0.0, 1.0, 0.001);
	dryWetSlider.setValue (processor.dryWet->get());
	dryWetLabel.attachToComponent(&dryWetSlider, false);
	dryWetLabel.setJustificationType(Justification::centred);

	
    setSize (800, 600);
}

ScatteringDelayReverbAudioProcessorEditor::~ScatteringDelayReverbAudioProcessorEditor()
{
}

//==============================================================================
void ScatteringDelayReverbAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
	g.fillAll (Colours::black);

    g.setColour (Colours::white);
    g.setFont (15.0f);
}

void ScatteringDelayReverbAudioProcessorEditor::sliderValueChanged (Slider* slider) {
	if(slider == &absorptionSlider) {
		processor.setAbsorption(slider->getValue());
	}
	
	if(slider == &dryWetSlider) {
		processor.dryWet->operator=(slider->getValue());
	}
};

void ScatteringDelayReverbAudioProcessorEditor::resized()
{
	roomContainer.setBoundsRelative(0.3, 0.1, 0.6, 0.8);
	absorptionSlider.setBoundsRelative(0.05, 0.1, 0.2, 0.3);
	dryWetSlider.setBoundsRelative(0.05, 0.5, 0.2, 0.3);
}
