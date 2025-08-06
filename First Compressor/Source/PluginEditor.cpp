/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilmStripSlider::FilmStripSlider(juce::Image* _knobStrip, int _frameCount, int _frameSize, bool _isVerticalStrip) {
    knobStrip = _knobStrip;
    frameSize = _frameSize;
    frameCount = _frameCount;
    isVerticalStrip = _isVerticalStrip;
}

void FilmStripSlider::drawFrame(juce::Graphics& g, int x, int y, int width, int height, juce::Slider& slider) {
    //  DBG("slider min: " << slider.getMinimum() << " slider max: " << slider.getMaximun());
      //float div = (float)((slider.getMaximun() - slider.getMinimun()) / frameCount);

     // DBG("slider val: " << slider.getValue());
      //double pos + (int)((slider.getValue() + slider.getMinimum()) / div;

    const double fractRotation = (slider.getValue() - slider.getMinimum()) / (slider.getMaximum() - slider.getMinimum()); //value between 0 and 1 for current amount of rotation
    int pos = (int)ceil(fractRotation * ((double)frameCount - 1.0)); // current index from 0 ---> nFrames-1

    slider.getRange();

    //if (pos > 0)
    // pos = pos - 1;

    if (width != height) /* scale */
    {
        x = (width / 2) - (height / 2);
        width = height;
    }

    if (isVerticalStrip)
    {
        g.drawImage(*knobStrip, x, y, width, height, 0, (int)(pos * frameSize), frameSize, frameSize, false);
    }
    else
    {
        g.drawImage(*knobStrip, x, y, width, height, (int)(pos * frameSize), 0, frameSize, frameSize, false);
    }
}

void OtherLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
    const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider)
{
   
    FilmStripSlider knobFeedbackDelay = FilmStripSlider(&knobAttackStrip, 20, 83, true);
    knobFeedbackDelay.drawFrame(g, x, y, width, height, slider);
}

void CompressorLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle,
    const float rotaryEndAngle, juce::Slider& slider)
{
    FilmStripSlider knobThreshold = FilmStripSlider(&knobThresholdStrip, 60, 52, true);
    knobThreshold.drawFrame(g, x, y, width, height, slider);
}


FirstCompressorAudioProcessorEditor::FirstCompressorAudioProcessorEditor (FirstCompressorAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), 
    verticalMeterL([&]() { return audioProcessor.getPeakValueL(); }), 
    verticalMeterR([&]() { return audioProcessor.getPeakValueR(); }),
    verticalOutputMeterL([&]() { return audioProcessor.getcompressedOutput(0); }),
    verticalOutputMeterR([&]() { return audioProcessor.getcompressedOutput(1); }),
    gainReductMeterL([&]() { return audioProcessor.getGainReduction(0); }),
    gainReductMeterR([&]() { return audioProcessor.getGainReduction(1); })
{
    peakDetectorGUI.setPeakDetectorObject(audioProcessor.getPeakDetectorObjectL());
    peakDetectorGUI.setPeakDetectorObject(audioProcessor.getPeakDetectorObjectR());
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
   // levelMeterLookAndFeel.setColour(foleys::LevelMeter::lmMeterGradientLowColour, juce::Colours::green);
  //  juce::LookAndFeel::setDefaultLookAndFeel(&levelMeterLookAndFeel);
  //  meterGUI.setLookAndFeel(&levelMeterLookAndFeel);
  //  levelMeterLookAndFeel.setupDefaultMeterColours();
    sldrThreshold.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    sldrThreshold.setTextValueSuffix("dB"); //Make the tralation between slider 0 to 1 to dB
    sldrThreshold.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    sldrThreshold.setColour(juce::Slider::textBoxTextColourId, juce::Colours::orangered);
    sldrThreshold.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    sldrThreshold.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::orangered);
    sldrThreshold.setLookAndFeel(&compressorLookAndFeel);
    addAndMakeVisible(sldrThreshold);
    thresholdParameterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (audioProcessor.apvts, "THRESHOLD", sldrThreshold);

    sldrRatio.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    sldrRatio.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    sldrRatio.setColour(juce::Slider::textBoxTextColourId, juce::Colours::orangered);
    sldrRatio.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    sldrRatio.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::orangered);
    sldrRatio.setLookAndFeel(&compressorLookAndFeel);
    addAndMakeVisible(sldrRatio);
   ratioParameterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RATIO", sldrRatio);

   sldrAttack.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
   sldrAttack.setTextValueSuffix("ms"); //Make the tralation between slider 0 to 1 to dB
   sldrAttack.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
   sldrAttack.setColour(juce::Slider::textBoxTextColourId, juce::Colours::orangered);
   sldrAttack.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
   sldrAttack.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::orangered);
   sldrAttack.setLookAndFeel(&otherLookAndFeel);
   addAndMakeVisible(sldrAttack);
   attackParameterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ATTACK", sldrAttack);

   sldrRelease.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
   sldrRelease.setTextValueSuffix("ms"); //Make the tralation between slider 0 to 1 to dB
   sldrRelease.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
   sldrRelease.setColour(juce::Slider::textBoxTextColourId, juce::Colours::orangered);
   sldrRelease.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
   sldrRelease.setColour(juce::Slider::textBoxOutlineColourId, juce::Colours::orangered);
   sldrRelease.setLookAndFeel(&otherLookAndFeel);
   addAndMakeVisible(sldrRelease);
   releaseParameterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RELEASE", sldrRelease);

   juce::Font myFont(20.0f);
   myFont.setTypefaceStyle("Bold");
   juce::Font myFontOutline(18.0f);
   myFontOutline.setTypefaceStyle("Bold");

   addAndMakeVisible(threshLabel);
   threshLabel.setText("Thresh", dontSendNotification);
   threshLabel.setColour(juce::Label::textColourId, juce::Colours::orangered);
   threshLabel.setFont(myFont);

   addAndMakeVisible(ratioLabel);
   ratioLabel.setText("Ratio", dontSendNotification);
   ratioLabel.setColour(juce::Label::textColourId, juce::Colours::orangered);
   ratioLabel.setFont(myFont);

   addAndMakeVisible(attackLabel);
   attackLabel.setText("ATTACK", dontSendNotification);
   attackLabel.setColour(juce::Label::textColourId, juce::Colours::yellowgreen);
   attackLabel.setFont(myFontOutline);

  //  meterGUI.setMeterSource(&audioProcessor.getMeterSource());
  //  addAndMakeVisible(meterGUI);
    setSize (700, 433);

    addAndMakeVisible(verticalMeterL);
    addAndMakeVisible(verticalMeterR);

    addAndMakeVisible(verticalOutputMeterL);
    addAndMakeVisible(verticalOutputMeterR);

    addAndMakeVisible(gainReductMeterL);
    addAndMakeVisible(gainReductMeterR);
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


    auto totalArea = getLocalBounds();
    auto innerArea = totalArea.reduced(50);
    auto leftArea = innerArea.removeFromLeft(300);
    auto rightArea = innerArea.removeFromRight(300);
    auto attRelLeftArea = leftArea.removeFromBottom(111);
    auto attRelRightArea = rightArea.removeFromBottom(111);
   
    g.setColour(juce::Colours::blue);
    g.drawRect(leftArea);
    g.setColour(juce::Colours::red);
    g.drawRect(rightArea);
    g.setColour(juce::Colours::greenyellow);
    g.drawRect(attRelLeftArea);
    g.setColour(juce::Colours::rebeccapurple);
    g.drawRect(attRelRightArea);


    g.fillAll(Colours::darkgrey);

    background = ImageCache::getFromMemory(BinaryData::engine_back_jpg, BinaryData::engine_back_jpgSize);
    g.drawImageWithin(background, 0, 0, getWidth(), getHeight(), RectanglePlacement::stretchToFit);

    juce::AffineTransform transform = juce::AffineTransform::scale(1.f).translated(-70, -30); // Scale 110%
    g.drawImageTransformed(background, transform);

}

void FirstCompressorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    //sldrThreshold.setBounds(70, 50, 265, 200);
    //sldrRatio.setBounds(410, 50, 265, 200);
    //sldrAttack.setBounds(70, 270, 100, 100);
    //sldrRelease.setBounds(410, 270, 100, 100);

    auto totalArea = getLocalBounds();
    auto innerArea = totalArea.reduced(50);

    auto leftArea = innerArea.removeFromLeft(300);
    auto leftCentreX = leftArea.getCentreX() - 70;
    auto leftCentreY = leftArea.getCentreY() - 80;

    auto rightArea = innerArea.removeFromRight(300);
    auto rightCentreX = rightArea.getCentreX() - 85;
    auto rightCentreY = rightArea.getCentreY() - 80;

    auto attRelLeftArea = leftArea.removeFromBottom(111);
    auto attRelLeftCentreX = attRelLeftArea.getCentreX();
    auto attRelLeftCentreY = attRelLeftArea.getCentreY() - 50;
    
    auto attRelRightArea = rightArea.removeFromBottom(111);
    auto attRelRightCentreX = attRelRightArea.getCentreX();
    auto attRelRightCentreY = attRelRightArea.getCentreY() - 50;

    sldrThreshold.setBounds(leftCentreX, leftCentreY, 140, 140);
    sldrRatio.setBounds(rightCentreX, rightCentreY, 140, 140);
    sldrAttack.setBounds(attRelLeftCentreX + 40, attRelLeftCentreY, 90, 90);
    sldrRelease.setBounds(attRelRightCentreX - 130, attRelRightCentreY, 90, 90);

    threshLabel.setBounds(leftCentreX + 42, leftCentreY - 30, 70, 25);
    ratioLabel.setBounds(rightCentreX + 42, rightCentreY - 30, 70, 25);
    attackLabel.setBounds(attRelLeftCentreX + 52, attRelLeftCentreY + 100, 60, 20);

    verticalMeterL.setBounds(40, 100, 30, 200);
    verticalMeterR.setBounds(75, 100, 30, 200);
    verticalOutputMeterL.setBounds(580, 100, 30, 200);
    verticalOutputMeterR.setBounds(615, 100, 30, 200);
    gainReductMeterL.setBounds(670, 100, 25, 50);
}

//void FirstCompressorAudioProcessorEditor::timerCallback()
//{
//    //verticalMeterL.setLevel(audioProcessor.getPeakValue());
//   // verticalMeterL.setLevel(audioProcessor.getRMSDecibelsL());
//   //verticalMeterR.setLevel(audioProcessor.getRMSDecibelsR());
//    verticalMeterL.setLevel(Decibels::gainToDecibels(audioProcessor.getPeakValueL()));
//    verticalMeterR.setLevel(Decibels::gainToDecibels(audioProcessor.getPeakValueR()));
//    verticalOutputMeterL.setLevel(audioProcessor.getRMSDecibelsOutR());
//    verticalOutputMeterR.setLevel(audioProcessor.getRMSDecibelsOutR());
//
//
//    verticalMeterL.repaint();
//    verticalMeterR.repaint();
//
//    verticalOutputMeterL.repaint();
//    verticalOutputMeterR.repaint();
//}