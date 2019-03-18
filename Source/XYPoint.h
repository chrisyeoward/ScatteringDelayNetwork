/*
  ==============================================================================

    XYPoint.h
    Created: 18 Mar 2019 9:56:38am
    Author:  Chris Yeoward

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class XYPoint    : public Component
{
public:
    XYPoint();
    ~XYPoint();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XYPoint)
};
