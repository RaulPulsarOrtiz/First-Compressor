/*
  ==============================================================================

    PeakDetector.h
    Created: 29 Mar 2025 6:44:46pm
    Author:  Raul

  ==============================================================================
*/
#pragma once
#include<JuceHeader.h>


class PeakDetector
{
public:
    PeakDetector() {

    }

    float process(float inputSample);

    // double getSampleRate();
private:
    double sampleRate{ 48000.0 };
    double windowLengthMs{ 2.0 };

    int windowLengthSamples{ 96 };
    int sampleCounter{ 0 };

    float maxPeak{ 0.0f };
    float smoothedPeak{ 0.0f };

    const float attackCoeff{ 0.1f };
    const float releaseCoeff{ 0.0003f };
};
