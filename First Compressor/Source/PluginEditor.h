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
#include "../../../../../../3 - 2021-2022 Third/SDA/Exercises/First Time/Practical05 Juce/modules/juce_core/system/juce_PlatformDefs.h"
#include "F:\UWE\2 - PORTFOLIO\1 - New Projects\First Compressor\First-Compressor\First Compressor\Source\dBMeter.h"
//==============================================================================
/**
*/
/** Class to take the knob images strip */
class FilmStripSlider
{
public:
    FilmStripSlider(juce::Image* _knobStrip, int _frameCount, int _frameSize, bool _isVerticalStrip);
    ~FilmStripSlider() {}

    /** This function workout the minimum and maximum values of the slider and it shows the corresponding frame of the image strip based on the current value */
    void drawFrame(juce::Graphics& g, int x, int y, int width, int height, juce::Slider& slider);

private:
    /** This image is a strip of the different images of the slider with every different value  */
    juce::Image* knobStrip = nullptr;

    /** Number of frame values on the slider */
    int frameCount = 0;

    /** Width of the image */
    int frameSize = 0;

    /** If that image is vertical or horizontal */
    bool isVerticalStrip = true;
};

/** Custom LookAndFeel class for delayTime and Feedback sliders */
class OtherLookAndFeel : public juce::LookAndFeel_V4
{
public:
    OtherLookAndFeel() //On the constructor we set up some default colours for different parts of the GUI
    {

    }
    ~OtherLookAndFeel() {}

    /** Get Image (binary data) from memory
    * @param BinaryData::BinaryData::attackKnob_png is the Binary Data of image for the Attack and Release sliders
    * @param BinaryData::attackKnob_pngSize is the size of the frame of each slider image in the strip
    */
    Image knobAttackStrip = ImageCache::getFromMemory(BinaryData::attackKnob_png, BinaryData::attackKnob_pngSize);

    /** This funcion draws the slider image for the delayTime and feedback, instead of drawing a standard JUCE slider */
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle,
        const float rotaryEndAngle, juce::Slider&) override; // (x, y, width, height) are passing from the slider's resize() function. 
                                                             //sliderPost is where the slider is from 0 to 1.
                                                             // rotaryAngle is the angle in radiants from the center of the slider to the start position or to the end position
};

/** Custom LookAndFeel class for filter sliders */
class CompressorLookAndFeel : public juce::LookAndFeel_V4
{
public:
    CompressorLookAndFeel() {}
    ~CompressorLookAndFeel() {}

    /** Get Image (binary data) from memory
    * @param BinaryData::ThresholdKnob_png is the Binary Data of image for the Threshold and the Ratio sliders
    * @param BinaryData::ThresholdKnob_pngSize is the size of the frame of each slider image in the strip
    */
    Image knobThresholdStrip = ImageCache::getFromMemory(BinaryData::ThresholdKnob_png, BinaryData::ThresholdKnob_pngSize);

    /** This funcion draws the slider image for the filter, instead of drawing a standard JUCE slider */
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle,
        const float rotaryEndAngle, juce::Slider&) override; // (x, y, width, height) are passing from the slider's resize() function. 
                                                             //sliderPost is where the slider is from 0 to 1.
                                                             // rotaryAngle is the angle in radiants from the center of the slider to the start position or to the end position
};

class FirstCompressorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FirstCompressorAudioProcessorEditor (FirstCompressorAudioProcessor&);
    ~FirstCompressorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:

    FirstCompressorAudioProcessor& audioProcessor;

    PeakDetectorGUI peakDetectorGUI; //Don't know if I need thisssssssssssss
    /**Slider declaration: */
    juce::Slider sldrThreshold, sldrRatio, sldrAttack, sldrRelease;
    /** ValueTreeState Attachments */
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdParameterAttachment, ratioParameterAttachment, attackParameterAttachment, releaseParameterAttachment;
    /**Custom Look&Feel for 'Threshold' adn 'ratio' slider */
    CompressorLookAndFeel compressorLookAndFeel;
    /**Custom Look&Feel for 'Attack' and 'Release' sliders */
    OtherLookAndFeel otherLookAndFeel;
    /**Objects from the vertical meter class for input and output */
    verticalMeter verticalMeterL, verticalMeterR, verticalOutputMeterL, verticalOutputMeterR;
    /**Objects from the gain reduction meter class for show the gain that is being reduce by the action of the compressor */
    gainRecuctionMeter gainReductMeterL, gainReductMeterR;
    /**Lables displayed around the sliders explaining what this sliders are */
    Label threshLabel, ratioLabel, attackLabel;
    /** GUI background image */
    Image background;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FirstCompressorAudioProcessorEditor)
};
