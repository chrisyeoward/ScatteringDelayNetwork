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
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
	setInterceptsMouseClicks(false, false);
}

XYPoint::~XYPoint()
{
}

void XYPoint::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

	g.fillAll (Colours::transparentBlack);   // clear the background

    g.setColour (Colours::white);
//    g.drawEllipse(0, 0, getWidth(), getHeight(), 5);   // draw an outline around the component
	g.fillEllipse(0, 0, getWidth(), getHeight());

    g.setColour (Colours::white);
    g.setFont (14.0f);
//    g.drawText ("XYPoint", getLocalBounds(),
//                Justification::centred, true);   // draw some placeholder text
}

void XYPoint::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
