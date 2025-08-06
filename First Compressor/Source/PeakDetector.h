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
    * @param inputSample gets a sample from the buffer and process it to detect the peaks in order to compress them
    * @return returns a smoothed value
    */
    float process(float inputSample); //This one is smoothed for metering
private: 

  //  double sampleRate{ 48000.0 };
 //   double windowLengthMs{ 2.0 };
   
    //Constants for metering
    //int windowLengthSamples{ 96 };
  //  int sampleCounter{ 0 };

  //  float maxPeak{ 0.0f };
    float smoothedPeak{ 0.0f };

    const float attackCoeff{ 0.1f };
    const float releaseCoeffMeter{ 0.0003f };

};
