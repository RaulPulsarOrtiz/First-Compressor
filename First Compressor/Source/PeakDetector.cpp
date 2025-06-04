/*
  ==============================================================================

    PeakDetector.cpp
    Created: 29 Mar 2025 6:44:46pm
    Author:  Raul

  ==============================================================================
*/

#include "PeakDetector.h"
void PeakDetector::setWindowLengthMs(int milisecons) {
    windowLengthSamples = static_cast<int>(milisecons * 0.001 * sampleRate);
    if (windowLengthSamples < 1) windowLengthSamples = 1;
    sampleCounter = 0;
    maxPeak = 0.0f;
}

void PeakDetector::setSampleRate(double newSampleRate)
{
    sampleRate = newSampleRate;
    setWindowLengthMs(windowLengthMs); // Recalculate with updated sample rate
}

//float PeakDetector::process(float inputSample)
//{
//    float absSample = std::abs(inputSample);
//    if (absSample > maxPeak)
//        maxPeak = absSample;
//
//    sampleCounter++;
//
//    if (sampleCounter >= windowLengthSamples)
//    {
//        // Smooth update
//        const float coeff = (maxPeak > smoothedPeak) ? attackCoeff : releaseCoeff;
//        smoothedPeak = coeff * maxPeak + (1.0f - coeff) * smoothedPeak;
//
//        // Reset window
//        maxPeak = 0.0f;
//        sampleCounter = 0;
//    }
//
//    return smoothedPeak;
//
//}

float PeakDetector::process(float inputSample)
{
    float absSample = std::abs(inputSample);
    const float coeff = (absSample > smoothedPeak) ? attackCoeff : releaseCoeff;
    smoothedPeak = coeff * absSample + (1.0f - coeff) * smoothedPeak;

    return smoothedPeak;
}
//double PeakDetector::getSampleRate()
//{
//    return sampleRate;
//}