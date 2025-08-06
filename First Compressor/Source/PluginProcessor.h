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

    PeakDetector* getPeakDetectorObjectL();
    PeakDetector* getPeakDetectorObjectR();
    float linearToDB(float linear);
    float dBToLinear(float dB);
    /**Uses the compress function to select the signal above the threhold and devide it by the ratio
    * @param Input signal after being detected and smooth it by the Peak Detector
    * @param Threshold above the signal will be compressed.
    * @param Ratio for reducing the signal above the threshold
    * @return It returns the gain reduction to be apply to the input signal
    */
    float compress(float input, float fThresh, float fRatio);
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    /** @return It returns the peaks of the left channel to be displayed on the meters, expressed in decibels */
    float getPeakValueL();
    /** @return It returns the peaks of the right channel to be displayed on the meters, expressed in decibels */
    float getPeakValueR();
    /** @return It returns the compressed signal after being ramped by the attack to be displayed on the meters, expressed in decibels.
    *  @param Selects if it is returning that value from the left channel or from the right channel.
    */
    float getcompressedOutput(int channel);
    /**@return It returns the gain reduction after being ramped by the attack, before being apply to the input signal to be displayed on the meter, expressed in decibels.
    * @param Selects if it is returning that value from the left channel or from the right channel.
    */
    float getGainReduction(int channel);

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
    PeakDetector peakDetectorL, peakDetectorR;
    PeakDetector outputMeterDetectorL, outputMeterDetectorR;

    float fThresh{ 0.f };
    float fRatio{ 0.f };
   // juce::SmoothedValue<float> attackReleaseRamp;
    juce::SmoothedValue<float> attackRampL, attackRampR;
    float fAttack{ 0.f };
    float previousfAttack{ 0.f };
    float newAttack{ 0.f };
    float fRelease{ 0.f };
    float previousfRelease{ 0.f };

    float peakL, peakR;
    float compressedOutputL, compressedOutputR;
    
    float outputMeterDataL, outputMeterDataR;
    float gainReductionRampedL, gainReductionRampedR;

    float gainReductionL, gainReductionR;

    bool attackTimeNeedsUpdate = false;
};
