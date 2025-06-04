/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "F:\UWE\2 - PORTFOLIO\1 - New Projects\First Compressor\First-Compressor\First Compressor\Source\PluginProcessor.h"

//==============================================================================
FirstCompressorAudioProcessor::FirstCompressorAudioProcessor()
    : AudioProcessor(juce::AudioProcessor::BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    apvts(*this, nullptr, "Parameters", createParameters()) // ✅ Now properly initialized
{
}

FirstCompressorAudioProcessor::~FirstCompressorAudioProcessor()
{
}

//==============================================================================
const juce::String FirstCompressorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FirstCompressorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FirstCompressorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FirstCompressorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FirstCompressorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FirstCompressorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FirstCompressorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FirstCompressorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FirstCompressorAudioProcessor::getProgramName (int index)
{
    return {};
}

void FirstCompressorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//foleys::LevelMeterSource& FirstCompressorAudioProcessor::getMeterSource()
//{
//    return meterSource;
//}

juce::AudioProcessorValueTreeState::ParameterLayout FirstCompressorAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params; //Vector to push new parameters
    params.push_back(std::make_unique<juce::AudioParameterFloat>("THRESHOLD", "Threshold", -60.f, 0.f, 0.0f)); //Threshold
    params.push_back(std::make_unique<juce::AudioParameterInt>("RATIO", "Ratio", 1, 20, 1)); //Ratio as Integer is a good idea?
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.01f, 0.2f, 0.01f)); // smoothValue want the ramp in second. Range from 10ms to 200ms atm
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.5f, 1.f, 0.4f));// smoothValue want the ramp in second. Range from 40ms to 1000ms atm
        return{ params.begin(), params.end() };


}
//==============================================================================
void FirstCompressorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
       // this prepares the meterSource to measure all output blocks and average over 100ms to allow smooth movements
   // meterSource.resize(getTotalNumOutputChannels(), sampleRate * 0.1 / samplesPerBlock);
    // ...
    peakDetector.setSampleRate(sampleRate);
    attackRamp.reset(sampleRate, fAttack);
   // gainReductionRamp.setCurrentAndTargetValue(1.0f);

}

void FirstCompressorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FirstCompressorAudioProcessor::isBusesLayoutSupported(const juce::AudioProcessor::BusesLayout& layouts) const
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

PeakDetector* FirstCompressorAudioProcessor::getPeakDetectorObject()
{
    return &peakDetector;
}

float FirstCompressorAudioProcessor::linearToDB(float linear) {
    return 20.0f * std::log10(linear);
}

float FirstCompressorAudioProcessor::dBToLinear(float dB) {
    return std::pow(10.0f, dB / 20.0f);
}

float FirstCompressorAudioProcessor::compress(float input, float fThresh, float fRatio) {
 
    float output{ 0.f };
    
    float thresholdLinear = dBToLinear(fThresh);
   // DBG("Threshold is: " << fThresh);
    //DBG("Threshold Linear" << thresholdLinear);
    if (input > thresholdLinear)
        output = thresholdLinear + (input - thresholdLinear) / fRatio;
    else
        output = input;
    return input == 0 ? 1.0f : output / input; //If input == 0, you avoid division by zero and return 1.0f
}

void FirstCompressorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    int numSamples = buffer.getNumSamples();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Measure the audio signal for the meter
  //  meterSource.measureBlock(buffer);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.

     // Fetch compressor parameters once per block
    
    fThresh = *apvts.getRawParameterValue("THRESHOLD");
    fRatio = *apvts.getRawParameterValue("RATIO");
    fAttack = *apvts.getRawParameterValue("ATTACK"); // / 1000.f; //SmoothValue wants the ramp in seconds no in samples
    fRelease = *apvts.getRawParameterValue("RELEASE"); // / 1000.f; //SmoothValue wants the ramp in seconds no in samples

    if (fAttack != previousfAttack)
    {
        attackRamp.reset(getSampleRate(), fAttack);
        previousfAttack = fAttack;
    }

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer(channel);

        for (int sample = 0; sample < numSamples; ++sample)
        {
            float* inputSample = (&channelData[sample]); // Get pointer to current sample   
            float peak = peakDetector.process(*inputSample);
            float gainReduction = compress(peak, fThresh, fRatio);
            attackRamp.setTargetValue(gainReduction);
            auto gainReductionRamped = attackRamp.getNextValue();
            *inputSample *= gainReductionRamped;
            //  buffer.addSample(channel, sample, signalCompressed);
                }

           
          //  buffer.addSample(channel, sample, signalCompressed);
        }
    
}


//==============================================================================
bool FirstCompressorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FirstCompressorAudioProcessor::createEditor()
{
    return new FirstCompressorAudioProcessorEditor (*this);
}

//==============================================================================
void FirstCompressorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FirstCompressorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FirstCompressorAudioProcessor();
}
