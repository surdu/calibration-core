#ifndef RhythmCore_h
#define RhythmCore_h

#include "Core.h"

class RhythmCore: public Core
{
  using Core::Core;

  public:
    void setup(CoreParams params);
    void drawLights(unsigned char peakCount);
    void playBackgroundTrack();
  private:
    unsigned char songIndex;
};

#endif
