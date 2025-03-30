/*
  ==============================================================================

    PeakDetectorGUI.h
    Created: 29 Mar 2025 6:50:51pm
    Author:  Raul

  ==============================================================================
*/

#pragma once
#include "PeakDetector.h"

class PeakDetectorGUI 
{
public:
    PeakDetectorGUI() { }
    ~PeakDetectorGUI() { }
    void setPeakDetectorObject(PeakDetector* peakDetector);

private:
    PeakDetector* peakDetectorPtr { nullptr };
};