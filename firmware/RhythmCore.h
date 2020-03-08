#ifndef RhythmCore_h
#define RhythmCore_h

#include "Core.h"

class RhythmCore: public Core
{
  using Core::Core;

  public:
    void drawLights(unsigned char peakCount);
};

#endif
