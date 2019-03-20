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
	absorptionSlider(Slider::LinearVertical, Slider::TextBoxAbove),
	absorptionLabel("", "Wall Absorption Amount: "),
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
//    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void ScatteringDelayReverbAudioProcessorEditor::sliderValueChanged (Slider* slider) {
	if(slider == &absorptionSlider) {
		processor.setAbsorption(slider->getValue());
	}
};

void ScatteringDelayReverbAudioProcessorEditor::resized()
{
	roomContainer.setBoundsRelative(0.3, 0.1, 0.6, 0.8);
	absorptionSlider.setBoundsRelative(0, 0.3, 0.2, 0.5);
//	sourceYPositionSlider.setBoundsRelative(0.5, 0.1, 0.5, 0.5);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
