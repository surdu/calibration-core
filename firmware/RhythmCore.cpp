#include "RhythmCore.h"

void RhythmCore::drawLights(unsigned char peakCount) {
	int leftLimit = (LED_COUNT / 2.0) - (peakCount / 2.0);
	int rightLimit = (LED_COUNT / 2.0) + (peakCount / 2.0);

	for (int f = 0; f <= LED_COUNT; f++) {
		if (f > leftLimit && f < rightLimit) {
			leds[f] = CRGB::Red;
		}
		else {
			leds[f] = CRGB::Black;
		}
	}
}
