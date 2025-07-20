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
        /* Remains of a past GUI */
        //   setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::azure); //Line of the Slider
        //   setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::goldenrod); //Line of the slider from 0 to the current value
        //   setColour(juce::Slider::thumbColourId, juce::Colours::transparentBlack); //Dot of the current value
        // setColour(juce::Slider::trackColourId, juce::Colours::cornflowerblue);
    }
    ~OtherLookAndFeel() {}

    /** Get Image (binary data) from memory
    * @param BinaryData::BinaryData::BOS_knob_DIVA_52x52_128f_png is the Binary Data of image for the delayTime knob and feedback slider (called sldrDelayTime and in the code)
    * @param BinaryData::BOS_knob_DIVA_52x52_128f_pngSize is the size of the frame of each slider image in the strip
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
    * @param BinaryData::Emu_Drummulator_png is the Binary Data of image for the Filter Cutoff slider (called sldrFreqCutoff and sldrDelayGain in the code)
    * @param BinaryData::Emu_Drummulator_pngSize is the size of the frame of each slider image in the strip
    */
    Image knobThresholdStrip = ImageCache::getFromMemory(BinaryData::ThresholdKnob_png, BinaryData::ThresholdKnob_pngSize);

    /** This funcion draws the slider image for the filter, instead of drawing a standard JUCE slider */
    void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle,
        const float rotaryEndAngle, juce::Slider&) override; // (x, y, width, height) are passing from the slider's resize() function. 
                                                             //sliderPost is where the slider is from 0 to 1.
                                                             // rotaryAngle is the angle in radiants from the center of the slider to the start position or to the end position
};

class FirstCompressorAudioProcessorEditor  : public juce::AudioProcessorEditor, public Timer
{
public:
    FirstCompressorAudioProcessorEditor (FirstCompressorAudioProcessor&);
    ~FirstCompressorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FirstCompressorAudioProcessor& audioProcessor;
    PeakDetectorGUI peakDetectorGUI;
 //   foleys::LevelMeter meterGUI; // The UI meter
//foleys::LevelMeterLookAndFeel levelMeterLookAndFeel;
  //  foleys::LevelMeter meterGUI{ foleys::LevelMeter::Minimal };
    juce::Slider sldrThreshold, sldrRatio, sldrAttack, sldrRelease;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> thresholdParameterAttachment, ratioParameterAttachment, attackParameterAttachment, releaseParameterAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FirstCompressorAudioProcessorEditor)


        /**Custom Look&Feel for 'Delay Time' and 'Feedback' sliders */
    OtherLookAndFeel otherLookAndFeel;

    /**Custom Look&Feel for 'Filter' slider */
    CompressorLookAndFeel compressorLookAndFeel;
    verticalMeter verticalMeterL, verticalMeterR, verticalOutputMeterL, verticalOutputMeterR;
};
