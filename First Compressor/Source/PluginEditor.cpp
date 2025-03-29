/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "F:\UWE\2 - PORTFOLIO\1 - New Projects\First Compressor\First-Compressor\First Compressor\Source\Meters\ff_meters.h"
#include "F:\UWE\2 - PORTFOLIO\1 - New Projects\First Compressor\First-Compressor\First Compressor\Source\Meters\LevelMeter\LevelMeterSource.h"
//==============================================================================
FirstCompressorAudioProcessorEditor::FirstCompressorAudioProcessorEditor (FirstCompressorAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
   // levelMeterLookAndFeel.setColour(foleys::LevelMeter::lmMeterGradientLowColour, juce::Colours::green);
  //  juce::LookAndFeel::setDefaultLookAndFeel(&levelMeterLookAndFeel);
  //  meterGUI.setLookAndFeel(&levelMeterLookAndFeel);
  //  levelMeterLookAndFeel.setupDefaultMeterColours();
    sldrThreshold.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    sldrThreshold.setTextValueSuffix("dB"); //Make the tralation between slider 0 to 1 to dB
    sldrThreshold.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
  //  sldrThreshold.setLookAndFeel(&otherLookAndFeel);
    addAndMakeVisible(sldrThreshold);
    thresholdParameterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "THRESHOLD", sldrThreshold);

    sldrRatio.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    // sldrRatio.setTextValueSuffix("dB"); //Make the tralation between slider 0 to 1 to dB
    sldrRatio.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    //  sldrRatio.setLookAndFeel(&otherLookAndFeel);
    addAndMakeVisible(sldrRatio);
    ratioParameterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RATIO", sldrRatio);

    meterGUI.setMeterSource(&audioProcessor.getMeterSource());
    addAndMakeVisible(meterGUI);
    setSize (400, 300);
}

FirstCompressorAudioProcessorEditor::~FirstCompressorAudioProcessorEditor()
{
    //juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
}

//==============================================================================
void FirstCompressorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void FirstCompressorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    sldrThreshold.setBounds(70, 50, 265, 200);
    sldrRatio.setBounds(410, 50, 265, 200);
}

