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
    /**@return This returns a the Peak Detector Object that is going to be used in the Editor constructor to make the Peak Detector GUI object to control parameters of this exact object and not any copy of it.
    * In this case Peak Detector GUI is doing nothing, but it is created in case I want to move some GUI functionality of the Peak Detector to this class
    * There is one for detecting the signal peaks on the left channel and another object for the right channel
    */
    PeakDetector* getPeakDetectorObjectL();
    PeakDetector* getPeakDetectorObjectR();
   
    /**This gets the logarithmic decibels value coming from the threshold sliders and convert them to linear values between 0 and 1 in order to use the compressor in the linear signal
    * In this way the threshold can express decibels values which is more common for the user
    * @return It take decibels values and return them as linear values between 0 and 1.
    */
    float dBToLinear(float dB);

    /**In case it would be needed, it takes linear values comming from the audio signal (between 0 and 1) and turn them into logarithmic decibels */
    float linearToDB(float linear);

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
  
    /** Declaration of the AudioProcessorValueTreeState object. This object holds all the parameters */
    juce::AudioProcessorValueTreeState apvts; //Try to make a function to keep this private

    /** Creates a list of parameters thar are linked with the sliders on a ValueTreeState
    * @return the list of parameters
    */
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters(); //This returns a list of the parameters 
private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FirstCompressorAudioProcessor)
    /**Peak Detector objects for stereo signal reading */
    PeakDetector peakDetectorL, peakDetectorR;
    PeakDetector outputMeterDetectorL, outputMeterDetectorR;
    /**Stores the value from Threshold parameter */
    float fThresh{ 0.f };
    /**Stores the value from Ratio parameter */
    float fRatio{ 0.f };
   // juce::SmoothedValue<float> attackReleaseRamp;
    /**Smoothed Value class from JUCE to apply the attack ramp. Stereo to be applied to two channels that could be different. The attack value will be the same choosen with the attack knob. */
    juce::SmoothedValue<float> attackRampL, attackRampR;
    /**Stores the value from Attack parameter */
    float fAttack{ 0.f };
    /**Stores the value from Previus attack parameter, needed for checking if the actual attack parameter is the same one of if it has changed */
    float previousfAttack{ 0.f };
    //float newAttack{ 0.f };
    /**Release parameter has not been fully implemented in this version. */
    float fRelease{ 0.f };
    float previousfRelease{ 0.f };
    /**Stores the value from Peaks signal after beeing smoothed */
    float peakL, peakR;
    /**Stores the value of the stereo compressed signal before it has been smoothed by the Peak Detector function. */
    float compressedOutputL, compressedOutputR;
    /**Stores the value of the stereo compressed signal after being smoothed by the Peak Detector function, in order to be display in the output meters */
    float outputMeterDataL, outputMeterDataR;
    /**The compressed value is a "gain reduction" value that need to be applied to the stereo input signal. These parameters are the result of that application before being ramped by the attack value.*/
    float gainReductionL, gainReductionR;
    /**The compressed value is a "gain reduction" value that need to be applied to the stereo input signal. These parameters are the result of that application after being ramped by the attack value.*/
    float gainReductionRampedL, gainReductionRampedR;
};
