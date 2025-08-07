/*
  ==============================================================================

    PeakDetector.h
    Created: 29 Mar 2025 6:44:46pm
    Author:  Raul

  ==============================================================================
*/
#pragma once
#include<JuceHeader.h>

/**Peak Detector class */
class PeakDetector
{
public:
    PeakDetector() {

    }

    /** 
    * @param inputSample gets a sample from the buffer and process it to detect the peaks in order to compress them.
    * It creates a coefficient depending if the next sample in the signal is increasing to catch those peaks or if it is decreasing to apply a slower smoothing value
    * @return returns a smoothed value more redeable than the signal sample per sample.
    */
    float process(float inputSample); //This one is smoothed for metering
private: 

  //  double sampleRate{ 48000.0 };
 //   double windowLengthMs{ 2.0 };
   
    //Constants for metering
    //int windowLengthSamples{ 96 };
  //  int sampleCounter{ 0 };

  //  float maxPeak{ 0.0f };
   /**Declaration of the variables to smooth the signal */
    float smoothedPeak{ 0.0f };

    const float attackCoeff{ 0.1f };
    const float releaseCoeffMeter{ 0.0003f };

};
