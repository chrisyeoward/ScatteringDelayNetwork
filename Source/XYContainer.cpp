/*
  ==============================================================================

    XYContainer.cpp
    Created: 18 Mar 2019 9:56:04am
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "XYContainer.h"

template <typename T>
T clamp(T value, T min, T max)
{
	return std::max(std::min(value, max), min);
}

template <typename T>
T square(T a, T b)
{
	return a * b;
}

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
	label << processor.roomSize->get() << " x ";
	label << processor.ROOM_HEIGHT << "m";
	roomSizeLabel.setText(label, dontSendNotification);
	
	float pointSize = 0.05 * getWidth();
	source.setSize(pointSize, pointSize); // set the size of the circle
	source.setCentrePosition(getSourceXCoordinate(), getSourceYCoordinate()); // update sourceXY position based on processor
	
	// draw mic as cross
	 g.setColour (Colours::white);
	float micX = getMicXCoordinate();
	float micY = getMicYCoordinate();
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
	const int xCoordinate = event.getMouseDownX() + event.getDistanceFromDragStartX();
	const int yCoordinate = event.getMouseDownY() + event.getDistanceFromDragStartY();

	const float xPosition = clamp(static_cast<float>(xCoordinate) / getWidth(), 0.f, 1.f) * processor.roomSize->get();
	const float yPosition = clamp(1.f - static_cast<float>(yCoordinate) / getHeight(), 0.f, 1.f) * processor.roomSize->get();

	const float distanceToSource = square(xCoordinate - getSourceXCoordinate()) + square(yCoordinate - getSourceYCoordinate());
	const float distanceToMic = square(xCoordinate - getMicXCoordinate()) + square(yCoordinate - getMicYCoordinate());

	if (distanceToSource < distanceToMic)
	{
		processor.updateSourcePosition(xPosition, yPosition, 1.5); // hard code z position to be 1.5 metres
	}
	else
	{
		processor.updateMicPosition(xPosition, yPosition, 1.5);
	}
}

float XYContainer::getSourceXCoordinate()
{
	return clamp(processor.sourceXPosition->get() / processor.roomSize->get(), 0.f, 1.f) * getWidth();
}

float XYContainer::getSourceYCoordinate()
{
	return clamp(1.f - processor.sourceYPosition->get() / processor.roomSize->get(), 0.f, 1.f) * getWidth();
}

float XYContainer::getMicXCoordinate()
{
	return clamp(processor.micXPosition->get() / processor.roomSize->get(), 0.f, 1.f) * getWidth();
}

float XYContainer::getMicYCoordinate()
{
	return clamp(1.f - processor.micYPosition->get() / processor.roomSize->get(), 0.f, 1.f) * getWidth();
}
