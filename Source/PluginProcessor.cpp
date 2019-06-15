/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
{
	
	addParameter (roomSize = new AudioParameterFloat ("roomSize", // parameter ID
															 "Room size", // parameter name
															 NormalisableRange<float> (0.0f, 100.0f),
															 5.0f)); // default value
	
	
	addParameter (absorption = new AudioParameterFloat ("absorption", // parameter ID
														  "Wall Reflectivity", // parameter name
														  NormalisableRange<float> (0.0f, 1.0),
														  0.85)); // default value
	
	addParameter (dryWet = new AudioParameterFloat ("dryWet", // parameter ID
														"Dry/Wet", // parameter name
														NormalisableRange<float> (0.0f, 1.0f),
														0.5f)); // default value
	
	addParameter (sourceXPosition = new AudioParameterFloat ("sourceXPosition", // parameter ID
															   "Source X Position", // parameter name
															   NormalisableRange<float> (0.0f, roomSize->get()),
															   roomSize->get()*0.9)); // default value
	
	addParameter (sourceYPosition = new AudioParameterFloat ("sourceYPosition", // parameter ID
															 "Source Y Position", // parameter name
															 NormalisableRange<float> (0.0f, roomSize->get()),
															 roomSize->get()/2)); // default value
	
	addParameter (micXPosition = new AudioParameterFloat ("micXPosition", // parameter ID
															 "Mic X Position", // parameter name
															 NormalisableRange<float> (0.0f, roomSize->get()),
															 roomSize->get()/10)); // default value
	
	addParameter (micYPosition = new AudioParameterFloat ("micYPosition", // parameter ID
															 "Mic Y Position", // parameter name
															 NormalisableRange<float> (0.0f, roomSize->get()),
															 roomSize->get()/2)); // default value
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	network = new SDN::Network(sampleRate, roomSize->get(), roomSize->get(), 3.0);
	network->setSourcePosition(sourceXPosition->get(), sourceYPosition->get(), 1.5);
	network->setMicPosition(micXPosition->get(), micYPosition->get(), 1.8);
	network->setAbsorptionAmount(absorption->get());
	
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
	
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear (i, 0, numSamples);


    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
	
		for (int i = 0; i < numSamples; ++i)
		{
			float in = 0.0;
			for (int channel = 0; channel < totalNumInputChannels; ++channel)
			{
				in += buffer.getReadPointer (channel)[i];
			}
			
			in /= totalNumInputChannels; // sum to mono
			
//			auto outDry = network->positionSource(in); // get stereo positioned source output (DRY)
//			auto outWet = network->scatterStereo(in); // get stereo reverb output (WET)
		
			// if position changes, get new positions
			// get struct of streams
			// set source/mic position
			
			auto outMono = network->scatterMono(in);
		
			buffer.getWritePointer (0)[i] = 0.0;
			buffer.getWritePointer (1)[i] = 0.0;

			float dryMix = fmin(dryWet->get() * 2, 1.0); // crossfade dry wet amounts (at 0.5 both have gain of 1)
			float wetMix = fmin((1 - dryWet->get()) * 2, 1.0);
			
//			float outL = dryMix * outDry.L + wetMix * outWet.L;
//			float outR = dryMix * outDry.R + wetMix * outWet.R;
			
			buffer.getWritePointer (0)[i] = outMono;
			buffer.getWritePointer (1)[i] = outMono;
			
//			if (outL <= 1 && outR <= 1) {
//				buffer.getWritePointer (0)[i] = outL;
//				buffer.getWritePointer (1)[i] = outR;
//			}
//			 else {
//				DBG("PEAKING"); // TO SAVE YOUR EARS
//			}
		}

        // ..do something to the data...
//    }
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

// method for updating source position only when slider changes
void ScatteringDelayReverbAudioProcessor::updateSourcePosition(float x, float y, float z)
{
	sourceXPosition->setValueNotifyingHost(x);
	sourceYPosition->setValueNotifyingHost(y);
	network->setSourcePosition(sourceXPosition->get(), sourceYPosition->get(), 1.5);
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
