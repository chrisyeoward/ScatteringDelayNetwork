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
	sourceXPositionSlider(Slider::RotaryVerticalDrag, Slider::TextBoxAbove),
	sourceYPositionSlider(Slider::RotaryVerticalDrag, Slider::TextBoxAbove),
	sourceXPositionLabel("", "X Position: "),
	sourceYPositionLabel("", "Y Position: "),
	processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	
	addAndMakeVisible (&sourceXPositionSlider);
	sourceXPositionSlider.addListener (this);
	sourceXPositionSlider.setRange (0.01, 4.99, 0.001);
	sourceXPositionSlider.setValue (processor.sourceXPosition->get());
	sourceXPositionLabel.attachToComponent(&sourceXPositionSlider, false);
	sourceXPositionLabel.setJustificationType(Justification::centred);
	
	addAndMakeVisible (&sourceYPositionSlider);
	sourceYPositionSlider.addListener (this);
	sourceYPositionSlider.setRange (0.01, 4.99, 0.001);
	sourceYPositionSlider.setValue (processor.sourceYPosition->get());
	sourceYPositionLabel.attachToComponent(&sourceYPositionSlider, false);
	sourceYPositionLabel.setJustificationType(Justification::centred);
	
    setSize (400, 400);
}

ScatteringDelayReverbAudioProcessorEditor::~ScatteringDelayReverbAudioProcessorEditor()
{
}

//==============================================================================
void ScatteringDelayReverbAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void ScatteringDelayReverbAudioProcessorEditor::sliderValueChanged (Slider* slider) {
//	if (slider == &sourceXPositionSlider) {
		processor.updateSourcePosition(sourceXPositionSlider.getValue(), sourceYPositionSlider.getValue(), 0);
//		processor.sourceXPosition->operator=((float) slider->getValue());
//	}
};

void ScatteringDelayReverbAudioProcessorEditor::resized()
{
	
	sourceXPositionSlider.setBoundsRelative(0, 0.1, 0.5, 0.5);
	sourceYPositionSlider.setBoundsRelative(0.5, 0.1, 0.5, 0.5);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
