/*
  ==============================================================================

    dBMeter.h
    Created: 17 Jul 2025 9:16:43pm
    Author:  Raul

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class verticalMeter : public Component, public Timer
{
public:
    verticalMeter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction)) //&& express that is a right value reference. The valueFucntion is move into the valueSupplier instead of copied. When I call valueSupplier I will get the latest value of the level
    {
        startTimer(24);
    }
    //Because it has a constructor, we need to initialise these instances within the constructor parent (Editor), and there is where we pass the function pointer

    void paint(Graphics& g) override
    {

        //Vertical
        const auto level = valueSupplier();

        auto bounds = getLocalBounds().toFloat();

     // Fill entire background with dark grey
        g.setColour(juce::Colours::grey);
        g.fillRoundedRectangle(bounds, 5.f);

        g.setGradientFill(gradient);

        //Map level from -60 / +6 to 0 / height
        const auto levelHeighth = jmap(level, -60.f, +6.f, 0.f, static_cast<float>(getHeight()));
       
        // Draw the white level bar from the left
        //juce::Rectangle<float> levelBar = bounds;
        //levelBar.setWidth(levelHeighth);
       
        g.fillRect(bounds.removeFromBottom(levelHeighth));
      //  g.setColour(juce::Colours::white);
     //   g.fillRoundedRectangle(levelBar, 5.f); //Here I set the Level to be the rectangle painted

       }

    void setLevel(float value)
    {
        DBG("Meter level: " << value);
        level = juce::jlimit(-60.f, 6.f, value);
       // repaint();
    }

    void resized() override
    {
         const auto bounds = getLocalBounds().toFloat();

         gradient = ColourGradient {
         Colours::green,
         bounds.getBottomLeft(),
         Colours::red,
         bounds.getTopLeft(),
         false
        };
        //I need to add yellow in the middle by adding a new colour to the gradient
        gradient.addColour(0.5, Colours::yellow);
    }

    void timerCallback() //Unique issue is that I will have a Timer for every instance of dBMeter created. (4 in this case)
    {
        repaint();
    }

private:
    float level = -60.f;
    std::function<float()> valueSupplier; //When execute it will give me the level value
    ColourGradient gradient;
};
