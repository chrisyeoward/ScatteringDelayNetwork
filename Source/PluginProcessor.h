/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Delay.h"
#include "Point.h"
#include "Constants.h"
#include "Boundary.h"
#include "Node.h"


//==============================================================================
/**
*/
class ScatteringDelayReverbAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    ScatteringDelayReverbAudioProcessor();
    ~ScatteringDelayReverbAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
	SDN::Point source = SDN::Point(1.0, 0.0);
	SDN::Point mic = SDN::Point(1.0, 6.0);
	
	SDN::Boundary bound1 = SDN::Boundary(0.0, SDN::Orientation::Y);
	SDN::Node node1 = SDN::Node(bound1.getScatteringNodePosition(mic, source));
	
	SDN::Boundary bound2 = SDN::Boundary(2.0, SDN::Orientation::Y);
	SDN::Node node2 = SDN::Node(bound2.getScatteringNodePosition(mic, source));
	
	SDN::Delay *sourceMicDelay;
	
	SDN::Delay *sourceNode1Delay;
	SDN::Delay *sourceNode2Delay;
	
	SDN::Delay *node1Node2Delay;
	SDN::Delay *node2Node1Delay;
	
	SDN::Delay *node1MicDelay;
	SDN::Delay *node2MicDelay;
	
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScatteringDelayReverbAudioProcessor)
};
