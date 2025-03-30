/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PeakDetectorGUI.h"
//#include "F:\UWE\2 - PORTFOLIO\1 - New Projects\First Compressor\First-Compressor\First Compressor\Source\Meters\LookAndFeel\LevelMeterLookAndFeel.h"

//==============================================================================
/**
*/
class FirstCompressorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FirstCompressorAudioProcessorEditor (FirstCompressorAudioProcessor&);
    ~FirstCompressorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FirstCompressorAudioProcessor& audioProcessor;
    PeakDetectorGUI peakDetectorGUI;
 //   foleys::LevelMeter meterGUI; // The UI meter
//foleys::LevelMeterLookAndFeel levelMeterLookAndFeel;
  //  foleys::LevelMeter meterGUI{ foleys::LevelMeter::Minimal };
    juce::Slider sldrThreshold, sldrRatio;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdParameterAttachment, ratioParameterAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FirstCompressorAudioProcessorEditor)
};
