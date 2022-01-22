/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class OverdriveAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    OverdriveAudioProcessorEditor (OverdriveAudioProcessor&);
    ~OverdriveAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    juce::Slider driveKnob;
    juce::Slider wetDryKnob;
    juce::Slider volumeKnob;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment; // Scoped pointer within treeState->sliderAttachment called "driveAttachment"
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wetDryAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volumeAttachment;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    OverdriveAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OverdriveAudioProcessorEditor)
};
