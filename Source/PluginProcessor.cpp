/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define STEREO_OUTPUT
//#define MUTE_OVER_0DBFS

const float ScatteringDelayReverbAudioProcessor::ROOM_HEIGHT = 3.f;
const float ScatteringDelayReverbAudioProcessor::MAX_ROOM_SIZE = 100.f;

//==============================================================================
ScatteringDelayReverbAudioProcessor::ScatteringDelayReverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
	,
	roomSizeLast(8.0f)
{
	network = new SDN::Network(44100); // placeholder network - to be reset before playback

	const float initSrcPosX = roomSizeLast / 2;
	const float initSrcPosY = roomSizeLast / 2;
	const float initMicPosX = roomSizeLast / 4;
	const float initMicPosY = roomSizeLast / 2;

	addParameter(roomSize = new AudioParameterFloat("roomSize", // parameter ID
		"Room size", // parameter name
		NormalisableRange<float>(0.0f, MAX_ROOM_SIZE),
		roomSizeLast)); // default value

	addParameter(absorption = new AudioParameterFloat("absorption", // parameter ID
		"Wall Reflectivity", // parameter name
		NormalisableRange<float>(0.0f, 1.0),
		0.5f)); // default value

	addParameter(dryWet = new AudioParameterFloat("dryWet", // parameter ID
		"Source X Position", // parameter name
		NormalisableRange<float>(0.0f, 1.0f),
		0.5f)); // default value

	addParameter(sourceXPosition = new AudioParameterFloat("sourceXPosition", // parameter ID
		"Source X Position", // parameter name
		NormalisableRange<float>(0.0f, MAX_ROOM_SIZE),
		initSrcPosX)); // default value

	addParameter(sourceYPosition = new AudioParameterFloat("sourceYPosition", // parameter ID
		"Source Y Position", // parameter name
		NormalisableRange<float>(0.0f, MAX_ROOM_SIZE),
		initSrcPosY)); // default value

	addParameter(micXPosition = new AudioParameterFloat("micXPosition", // parameter ID
		"Mic X Position", // parameter name
		NormalisableRange<float>(0.0f, MAX_ROOM_SIZE),
		initMicPosX)); // default value

	addParameter(micYPosition = new AudioParameterFloat("micYPosition", // parameter ID
		"Mic Y Position", // parameter name
		NormalisableRange<float>(0.0f, MAX_ROOM_SIZE),
		initMicPosY)); // default value
}

ScatteringDelayReverbAudioProcessor::~ScatteringDelayReverbAudioProcessor()
{
}

//==============================================================================
const String ScatteringDelayReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ScatteringDelayReverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ScatteringDelayReverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ScatteringDelayReverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ScatteringDelayReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ScatteringDelayReverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ScatteringDelayReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ScatteringDelayReverbAudioProcessor::setCurrentProgram (int index)
{
}

const String ScatteringDelayReverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void ScatteringDelayReverbAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ScatteringDelayReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	resetNetwork();

	DBG("azimuths");
	DBG(network->getSourceAzimuth());
	for(int node = 0; node < 6; node++) {
		DBG(network->getNodeAzimuth(node));
	}
	
	DBG("elevations");
	DBG(network->getSourceElevation());
	for(int node = 0; node < 6; node++) {
		DBG(network->getNodeElevation(node));
	}
}

void ScatteringDelayReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ScatteringDelayReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ScatteringDelayReverbAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
	const int numSamples = buffer.getNumSamples(); // How many samples in the buffer

	if (roomSize->get() != roomSizeLast)
	{
		resetNetwork();
		roomSizeLast = roomSize->get();
	}

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, numSamples);

	for (int i = 0; i < numSamples; ++i)
	{
		float in = 0.0;
		for (int channel = 0; channel < totalNumInputChannels; ++channel)
		{
			in += buffer.getReadPointer(channel)[i];
		}

		in /= totalNumInputChannels; // sum to mono

		SDN::StereoOutput outDry = network->positionSource(in); // get stereo positioned source output (DRY)
		SDN::StereoOutput outWet;
		float outL, outR;

#ifdef STEREO_OUTPUT
		outWet = network->scatterStereo(in); // get stereo reverb output (WET)
#else
		outWet.L = outWet.R = network->scatterMono(in / 3.0);
#endif

		float dryMix = fmin(dryWet->get() * 2, 1.0); // crossfade dry wet amounts (at 0.5 both have gain of 1)
		float wetMix = fmin((1 - dryWet->get()) * 2, 1.0);

		outL = dryMix * outDry.L + wetMix * outWet.L;
		outR = dryMix * outDry.R + wetMix * outWet.R;

#ifdef MUTE_OVER_0DBFS
		if (outL <= 1 && outR <= 1) {
#endif

			buffer.getWritePointer(0)[i] = outL;
			buffer.getWritePointer(1)[i] = outR;

#ifdef MUTE_OVER_0DBFS
		}
		else {
			buffer.getWritePointer(0)[i] = 0.0f;
			buffer.getWritePointer(1)[i] = 0.0f;
			DBG("PEAKING"); // TO SAVE YOUR EARS
		}
#endif
	}
}

//==============================================================================
bool ScatteringDelayReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ScatteringDelayReverbAudioProcessor::createEditor()
{
    return new ScatteringDelayReverbAudioProcessorEditor (*this);
}

//==============================================================================
void ScatteringDelayReverbAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ScatteringDelayReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void ScatteringDelayReverbAudioProcessor::resetNetwork()
{
	delete network;

	const float width = roomSize->get();
	const float length = roomSize->get();
	const float height = ROOM_HEIGHT;

	// ensure that source & mic are within room bounds
	const float srcX = std::min(length, sourceXPosition->get());
	const float srcY = std::min(width, sourceYPosition->get());
	const float micX = std::min(length, micXPosition->get());
	const float micY = std::min(width, micYPosition->get());

	sourceXPosition->setValueNotifyingHost(sourceXPosition->getNormalisableRange().convertTo0to1(srcX));
	sourceYPosition->setValueNotifyingHost(sourceYPosition->getNormalisableRange().convertTo0to1(srcY));
	micXPosition->setValueNotifyingHost(micXPosition->getNormalisableRange().convertTo0to1(micX));
	micYPosition->setValueNotifyingHost(micYPosition->getNormalisableRange().convertTo0to1(micY));

	network = new SDN::Network(getSampleRate(), width, length, height);
	network->setSourcePosition(srcX, srcY, height / 2);
	network->setMicPosition(micX, micY, height / 2);
	network->setAbsorptionAmount(absorption->get());
}

// method for updating source position only when slider changes
void ScatteringDelayReverbAudioProcessor::updateSourcePosition(float x, float y, float z)
{
	sourceXPosition->setValueNotifyingHost(sourceXPosition->getNormalisableRange().convertTo0to1(x));
	sourceYPosition->setValueNotifyingHost(sourceYPosition->getNormalisableRange().convertTo0to1(y));
	network->setSourcePosition(x, y, z);

	DBG("azimuths");
	DBG(network->getSourceAzimuth());
	for(int node = 0; node < 6; node++) {
		DBG(network->getNodeAzimuth(node));
	}
	
	DBG("elevations");
	DBG(network->getSourceElevation());
	for(int node = 0; node < 6; node++) {
		DBG(network->getNodeElevation(node));
	}
}

void ScatteringDelayReverbAudioProcessor::updateMicPosition(float x, float y, float z)
{
	micXPosition->setValueNotifyingHost(micXPosition->getNormalisableRange().convertTo0to1(x));
	micYPosition->setValueNotifyingHost(micYPosition->getNormalisableRange().convertTo0to1(y));
	network->setMicPosition(x, y, z);
}

void ScatteringDelayReverbAudioProcessor::setAbsorption(const float amount) {
	absorption->setValueNotifyingHost(amount);
	network->setAbsorptionAmount(amount);
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ScatteringDelayReverbAudioProcessor();
}
