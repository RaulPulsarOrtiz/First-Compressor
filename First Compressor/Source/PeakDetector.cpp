/*
  ==============================================================================

    PeakDetector.cpp
    Created: 29 Mar 2025 6:44:46pm
    Author:  Raul

  ==============================================================================
*/

#include "PeakDetector.h"

float PeakDetector::process(float inputSample)
{
    float absSample = std::abs(inputSample);
    const float coeff = (absSample > smoothedPeak) ? attackCoeff : releaseCoeffMeter; //To know if the singal is increasing or decreasing 
    smoothedPeak = coeff * absSample + (1.0f - coeff) * smoothedPeak; //Copy of the first-order IIR smoothing filter. This line blends the current input with the previous output, weighted by a coefficient (coeff) to create a smoothed output. It’s a one-pole low-pass filter in disguise.

    return smoothedPeak;
}