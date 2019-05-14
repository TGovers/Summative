/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SummativeAudioProcessorEditor  : public AudioProcessorEditor, Slider::Listener
{
public:
    SummativeAudioProcessorEditor (SummativeAudioProcessor&);
    ~SummativeAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    
    Slider mixSlider;
    Label mixLabel;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
    
    Slider freqSlider;
    Label freqLabel;
    
    void sliderValueChanged(Slider * slider) override;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SummativeAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SummativeAudioProcessorEditor)
};
