/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OverdriveAudioProcessor::OverdriveAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), grantre (*this, nullptr, "Parameters", createParameters())

// Arguments: this points to the class, nullptr since we are not using UndoManager, ID name of the value tree, createParameters returns the layout from the function at the bottom


#endif
{
    

}

OverdriveAudioProcessor::~OverdriveAudioProcessor()
{
}

//==============================================================================
const juce::String OverdriveAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OverdriveAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OverdriveAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OverdriveAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OverdriveAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OverdriveAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OverdriveAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OverdriveAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OverdriveAudioProcessor::getProgramName (int index)
{
    return {};
}

void OverdriveAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void OverdriveAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void OverdriveAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OverdriveAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
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

void OverdriveAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    

    
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    float Drive = *grantre.getRawParameterValue("DRIVE");
    float wetDry = *grantre.getRawParameterValue("WETDRY");
    float Volume = *grantre.getRawParameterValue("VOLUME");
    float driveMag = db2mag(Drive);
    float volMag = db2mag(Volume);
    const float** in = buffer.getArrayOfReadPointers();
    float** out = buffer.getArrayOfWritePointers();
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel) {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            float curSamp = (in[channel][sample] + last[channel])/2.0;
            curSamp *= driveMag;
            curSamp = (2.0 / juce::float_Pi) * atan(curSamp);
            curSamp /= driveMag;
            curSamp = curSamp * wetDry + in[channel][sample] * (1 - wetDry);
            curSamp *= volMag;
            out[channel][sample] = curSamp;
            last[channel]  = in[channel][sample];
        }
    }
    
    
}



//==============================================================================
bool OverdriveAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OverdriveAudioProcessor::createEditor()
{
    return new OverdriveAudioProcessorEditor (*this);
}

//==============================================================================
void OverdriveAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    
    
}

void OverdriveAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OverdriveAudioProcessor();
}


juce::AudioProcessorValueTreeState::ParameterLayout OverdriveAudioProcessor::createParameters() // The function that returns the parameter layout
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params; // a list of *ranged parameters called "params"
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRIVE", "Drive", -12.0f, 30.f, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("WETDRY", "wetDry", 0.0f, 1.0f, 0.5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("VOLUME", "Volume", -12.0f, 12.0f, 1.f));
    /* Pushback since we want to end in the end of the vector. Arguments: DRIVE is ID, Drive is what you will see in the DAW, minimum value, maximum value, default value */
    
    
    return { params.begin(), params.end() }; // Return the vector - give the parameter layout
    
}




float OverdriveAudioProcessor::db2mag(float db) {
    db = pow(10.f, db / 20.f);
    
    return db;
}
