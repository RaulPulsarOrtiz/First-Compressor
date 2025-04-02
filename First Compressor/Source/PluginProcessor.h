/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PeakDetector.h"

//==============================================================================
/**
*/
class FirstCompressorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    FirstCompressorAudioProcessor();
    ~FirstCompressorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    PeakDetector* getPeakDetectorObject();
    float linearToDB(float linear);
    float dBToLinear(float dB);
    float compress(float input, float fThresh, float fRatio);
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
 //   foleys::LevelMeterSource& getMeterSource();
    juce::AudioProcessorValueTreeState apvts; //Try to make a function to keep this private
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters(); //This returns a list of the parameters 
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FirstCompressorAudioProcessor)
 //   foleys::LevelMeterSource meterSource;
    PeakDetector peakDetector;
   // float fThresh = 0.f;
   // float fRatio = 0.f;
    float fThresh{ 0.f };
    float fRatio{ 0.f };
    
};
