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
        iMeasuredLength = 0;
        iMeasuredItems = 0;
        fMax = 0;
        fMaxOld = 0;
        fMaxPrevious = 0;
        fAval = 0;
        fMaxNew = 0;
    }

    void setWindowLength(int milisecons) {
        iMeasuredLength = (int)(milisecons * 0.02 * sampleRate);
    }

    float process(float input) {

        fAval = fabs(input); //Input stored as absolute value (always positive)

        fMaxOld = fAval;

        if (fAval > fMaxOld) {
            fMax = fAval;
        }
        else {
            fMax = fMaxOld;
        }

        for (int i = 0; i < iMeasuredLength; i++) {
            iMeasuredItems++;
        }
        if (iMeasuredItems = iMeasuredLength)
        {
            //Scale
            fMax = (fMax * (40 - 1)) + 1;

            //Log
            fMax = log(fMax);

            //Scale to 0 - 1 Graph again
            fMax = fMax / log(40);

            fMaxPrevious = fMax; // Remeber Last value

            iMeasuredItems = 0;
            fMax = 0;

            float coeff = (fMaxNew > fMaxPrevious) ? 0.1 : 0.0003;
            return fMaxPrevious = coeff * fMaxNew + (1 - coeff) * fMaxOld;
        }
    }
    void setSampleRate(double newSampleRate);
   // double getSampleRate();
private:
    int iMeasuredLength;
    int iMeasuredItems;
    float fMax;
    float fMaxOld;
    float fMaxPrevious;
    float fAval;
    float fMaxNew; // filter will gradually work from fMAxPrevious to thi value
    
    double sampleRate{ 48000 };
};
