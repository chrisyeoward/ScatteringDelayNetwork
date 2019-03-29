/*
  ==============================================================================

    XYPoint.cpp
    Created: 18 Mar 2019 9:56:38am
    Author:  Chris Yeoward

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "XYPoint.h"

//==============================================================================
XYPoint::XYPoint()
{
	
	setInterceptsMouseClicks(false, false); // propagate mouse drag to parent component
}

XYPoint::~XYPoint()
{
}

void XYPoint::paint (Graphics& g)
{

	g.fillAll (Colours::transparentBlack);   // clear the background

    g.setColour (Colours::white);
	g.fillEllipse(0, 0, getWidth(), getHeight());

    g.setColour (Colours::white);
    g.setFont (14.0f);
}

void XYPoint::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
