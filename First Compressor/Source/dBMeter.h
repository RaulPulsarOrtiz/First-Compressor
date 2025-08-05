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
       
        // Meter fill (simplified example)
        g.fillRect(bounds.removeFromBottom(levelHeighth));
      //  g.setColour(juce::Colours::white);
     //   g.fillRoundedRectangle(levelBar, 5.f); //Here I set the Level to be the rectangle painted

        // Draw dB labels
        g.setColour(Colours::white);
        g.setFont(10.0f);

        const int numSteps = 6;
        for (int i = 0; i <= numSteps; ++i) //Each iteration draws one label at a different vertical position.
        {
            float normY = i / (float)numSteps; //Converts i into a normalized value between 0.0 and 1.0. For i = 0, normY = 0.0 (top of the meter). For i = 6, normY = 1.0 (bottom of the meter).       
            
            //float y = bounds.getY() + bounds.getHeight() * normY; //Calculates the Y-position in pixels where the label will be drawn.
            float y = 5 + 200 * normY;
            int dBValue = juce::jmap(normY, 0.0f, 1.0f, 0.0f, -60.0f); // Example from 0 to -60 dB. Converts the normalized normY into a dB label value using juce::jmap().
            g.drawText(String(dBValue) + "dB", 2, (int)y - 6, 40, 12, Justification::left); 
        }
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

class gainRecuctionMeter : public Component, public Timer
{
public:
    gainRecuctionMeter(std::function<float()>&& valueFunction) : valueSupplier(std::move(valueFunction)) //&& express that is a right value reference. The valueFucntion is move into the valueSupplier instead of copied. When I call valueSupplier I will get the latest value of the level
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

        //Map level from -30 / +6 to 0 / height
        const float levelHeight = juce::jmap(level, 0.f, -30.f, 0.f, static_cast<float>(getHeight()));
        // Clamp to avoid overfill:
        const float clampedHeight = juce::jlimit(0.f, (float)getHeight(), levelHeight);

        // Create a fill rectangle starting from the top
        juce::Rectangle<float> fillRect = bounds;
        fillRect.setHeight(clampedHeight);

        g.fillRect(fillRect);
      
          // Draw dB labels
        g.setColour(Colours::white);
        g.setFont(10.0f);

        const int numSteps = 2;
        for (int i = 0; i <= numSteps; ++i) //Each iteration draws one label at a different vertical position.
        {
            float normY = i / (float)numSteps; //Converts i into a normalized value between 0.0 and 1.0. For i = 0, normY = 0.0 (top of the meter). For i = 6, normY = 1.0 (bottom of the meter).       

            //float y = bounds.getY() + bounds.getHeight() * normY; //Calculates the Y-position in pixels where the label will be drawn.
            float y = 5 + 50 * normY; //This is the size of the meters
            int dBValue = juce::jmap(normY, 0.0f, 1.0f, 0.0f, -30.0f); // Example from 0 to -60 dB. Converts the normalized normY into a dB label value using juce::jmap().
            g.drawText(String(dBValue) + "dB", 2, (int)y - 6, 40, 12, Justification::left);
        }
    }

    void setLevel(float value)
    {
        DBG("Meter level: " << value);
        level = juce::jlimit(-30.f, 6.f, value);
        // repaint();
    }

    void resized() override
    {
        const auto bounds = getLocalBounds().toFloat();

        gradient = ColourGradient{
        Colours::red,
        bounds.getBottomLeft(),
        Colours::green,
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
    float level = -30.f;
    std::function<float()> valueSupplier; //When execute it will give me the level value
    ColourGradient gradient;
};