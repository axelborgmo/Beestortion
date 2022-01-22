/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OverdriveAudioProcessorEditor::OverdriveAudioProcessorEditor (OverdriveAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    
    /* Drive knob */
    driveKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    driveKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);
    addAndMakeVisible(driveKnob);

    driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.grantre, "DRIVE", driveKnob); // Allocates storage and connects
    
    
    /* Wet dry knob */
    wetDryKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    wetDryKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);
    addAndMakeVisible(wetDryKnob);
        
    wetDryAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.grantre, "WETDRY", wetDryKnob); // Allocates storage and connects
    
    
    /* Volume knob */
    volumeKnob.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    volumeKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 100, 100);
    addAndMakeVisible(volumeKnob);
    
    volumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.grantre, "VOLUME", volumeKnob); // Allocates storage and connects

    setSize (1000, 500);
}

OverdriveAudioProcessorEditor::~OverdriveAudioProcessorEditor()
{
}

//==============================================================================
void OverdriveAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::white);
    
    getLookAndFeel().setColour (juce::Slider::thumbColourId, juce::Colours::gold);
    juce::Image background = juce::ImageCache::getFromMemory (BinaryData::beestortion_jpeg, BinaryData::beestortion_jpegSize);
    g.drawImageAt (background, 0, 0);
    
    g.setColour (juce::Colours::black);
    g.setFont (juce::Font ("Arial Black", 80.0f, juce::Font::bold));
    g.drawText("BEESTORTION", getLocalBounds(), juce::Justification::centredTop, true);
    

    g.setFont (juce::Font ("Arial", 30.0f, juce::Font::plain));
    g.drawText ("Drive", 150, 160, 200, 40, juce::Justification::centred, true);
    g.drawText ("Wet/Dry", 400, 160, 200, 40, juce::Justification::centred, true);
    g.drawText ("Volume", 650, 160, 200, 40, juce::Justification::centred, true);

}

void OverdriveAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
        
    
    driveKnob.setBounds(((getWidth() / 4) * 1) - 210, (getHeight() / 2 - 50), getWidth() / 2 - 75, getHeight() / 2 - 75);
    wetDryKnob.setBounds(((getWidth() / 4) * 2) - 210, (getHeight() / 2 - 50), getWidth() / 2 - 75, getHeight() / 2 - 75);
    volumeKnob.setBounds(((getWidth() / 4) * 3) - 210, (getHeight() / 2 - 50), getWidth() / 2 - 75, getHeight() / 2 - 75);
    
}




