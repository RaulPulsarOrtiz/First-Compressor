/*
  ==============================================================================

    dBMeter.h
    Created: 17 Jul 2025 9:16:43pm
    Author:  Raul

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class verticalMeter : public Component
{
public:
    void paint(Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat();

     // Fill entire background with dark grey
        g.setColour(juce::Colours::grey);
        g.fillRoundedRectangle(bounds, 5.f);

        g.setColour(Colours::white); //Map level from -60 / +6 to 0 / height
        const auto levelWidth = jmap(level, -60.f, +6.f, 0.f, static_cast<float>(getWidth()));
       
        // Draw the white level bar from the left
        juce::Rectangle<float> levelBar = bounds;
        levelBar.setWidth(levelWidth);
       
        g.setColour(juce::Colours::white);
        g.fillRoundedRectangle(levelBar, 5.f);
       }

    void setLevel(float value)
    {
        DBG("Meter level: " << value);
        level = juce::jlimit(-60.f, 6.f, value);
       // repaint();
    }
private:
    float level = -60.f;
};
