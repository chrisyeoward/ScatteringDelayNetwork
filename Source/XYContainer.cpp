/*
  ==============================================================================

    XYContainer.cpp
    Created: 18 Mar 2019 9:56:04am
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "XYContainer.h"

//==============================================================================
XYContainer::XYContainer(ScatteringDelayReverbAudioProcessor &p) : processor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	addAndMakeVisible(source);
	startTimerHz(30);
	setRepaintsOnMouseActivity(true);
}

XYContainer::~XYContainer()
{
}

void XYContainer::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
//    g.drawText ("XYContainer", getLocalBounds(),
//                Justification::centred, true);   // draw some placeholder text
	
	g.setColour (Colours::grey.darker());
	g.drawLine(0.25*getWidth(), 0, 0.25*getWidth(), getHeight());
	g.drawLine(0.5*getWidth(), 0, 0.5*getWidth(), getHeight());
	g.drawLine(0.75*getWidth(), 0, 0.75*getWidth(), getHeight());
	
	g.drawLine(0, 0.25*getHeight(), getWidth(), 0.25*getHeight());
	g.drawLine(0, 0.5*getHeight(), getWidth(), 0.5*getHeight());
	g.drawLine(0, 0.75*getHeight(), getWidth(), 0.75*getHeight());
	
	float pointSize = 0.05 * getWidth();
	source.setSize(pointSize, pointSize);
	
	float x = processor.sourceXPosition->convertTo0to1(processor.sourceXPosition->get());
	float y = 1 - processor.sourceYPosition->convertTo0to1(processor.sourceYPosition->get());
	source.setCentreRelative(x, y);
	
	// draw mic
	 g.setColour (Colours::white);
	float micX = processor.micXPosition->convertTo0to1(processor.micXPosition->get()) * getWidth();
	float micY = (1 - processor.micYPosition->convertTo0to1(processor.micYPosition->get())) * getHeight();
	g.drawLine(micX - pointSize/2, micY - pointSize/2, micX + pointSize/2, micY + pointSize/2);
	g.drawLine(micX - pointSize/2, micY + pointSize/2, micX + pointSize/2, micY - pointSize/2);
	
}

void XYContainer::resized()
{
//	float pointSize = 0.05;
//	source.setSize(pointSize*getWidth(), pointSize*getWidth());
//
//	float x = processor.sourceXPosition->convertTo0to1(processor.sourceXPosition->get());
//	float y = processor.sourceYPosition->convertTo0to1(processor.sourceYPosition->get());
//	source.setCentreRelative(x, y);
//	source.setBoundsRelative(0.5, 0.5, pointSize, pointSize);
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void XYContainer::timerCallback()
{
	repaint();
}

void XYContainer::mouseDrag(const MouseEvent& event)
{
	
		float x = (event.getMouseDownX() + event.getDistanceFromDragStartX()) / (float) getWidth();
		float y = 1 - ((event.getMouseDownY() + event.getDistanceFromDragStartY()) / (float) getHeight());
		
		processor.updateSourcePosition(x, y, 1.5);
//		repaint();
}
