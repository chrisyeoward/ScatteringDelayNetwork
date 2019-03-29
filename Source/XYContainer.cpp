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
XYContainer::XYContainer(ScatteringDelayReverbAudioProcessor &p) :
	roomSizeLabel("", "Room size"),
	processor(p)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	addAndMakeVisible(source);
	startTimerHz(30);
	setRepaintsOnMouseActivity(true);
	
	addAndMakeVisible(roomSizeLabel);
	
	roomSizeLabel.setSize(100, 20);
	roomSizeLabel.setJustificationType(Justification::centred);

}

XYContainer::~XYContainer()
{
}


void XYContainer::paint (Graphics& g)
{
    /*
	 Draw canvas
	 
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

	
    g.setColour (Colours::white);
    g.setFont (14.0f);
	
	g.setColour (Colours::grey.darker());
	g.drawLine(0.25*getWidth(), 0, 0.25*getWidth(), getHeight());
	g.drawLine(0.5*getWidth(), 0, 0.5*getWidth(), getHeight());
	g.drawLine(0.75*getWidth(), 0, 0.75*getWidth(), getHeight());
	
	g.drawLine(0, 0.25*getHeight(), getWidth(), 0.25*getHeight());
	g.drawLine(0, 0.5*getHeight(), getWidth(), 0.5*getHeight());
	g.drawLine(0, 0.75*getHeight(), getWidth(), 0.75*getHeight());
	
	
	String label;
	label << processor.roomSize->get() << " x ";
	label << processor.roomSize->get() << " x 3m";
	roomSizeLabel.setText(label, dontSendNotification);
	
	float pointSize = 0.05 * getWidth();
	source.setSize(pointSize, pointSize); // set the size of the circle
	
	float x = processor.sourceXPosition->convertTo0to1(processor.sourceXPosition->get());
	float y = 1 - processor.sourceYPosition->convertTo0to1(processor.sourceYPosition->get()); // flip so y = 0 is at bottom
	source.setCentreRelative(x, y); // update sourceXY position based on processor
	
	// draw mic as cross
	 g.setColour (Colours::white);
	float micX = processor.micXPosition->convertTo0to1(processor.micXPosition->get()) * getWidth();
	float micY = (1 - processor.micYPosition->convertTo0to1(processor.micYPosition->get())) * getHeight();
	g.drawLine(micX - pointSize/2, micY - pointSize/2, micX + pointSize/2, micY + pointSize/2);
	g.drawLine(micX - pointSize/2, micY + pointSize/2, micX + pointSize/2, micY - pointSize/2);
	
	g.setColour (Colours::grey);
	g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void XYContainer::resized()
{


}

void XYContainer::timerCallback()
{
	repaint();
}

void XYContainer::mouseDrag(const MouseEvent& event)
{
	
		float x = (event.getMouseDownX() + event.getDistanceFromDragStartX()) / (float) getWidth();
		float y = 1 - ((event.getMouseDownY() + event.getDistanceFromDragStartY()) / (float) getHeight());
		
		processor.updateSourcePosition(x, y, 1.5); // hard code z position to be 1.5 metres
}
