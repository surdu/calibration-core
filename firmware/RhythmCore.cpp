#include "RhythmCore.h"

unsigned char song[] = {
  3, 3, 3, 3, 1, 1, 1, 1, 2, 2, 2, 2
};

size_t songLength = sizeof(song) / sizeof(unsigned char);

DEFINE_GRADIENT_PALETTE( colorMap ) {
		0,   190, 166,  36,
	175,    35,  82, 182,
	255,    20, 200, 214,
};

CRGBPalette16 palette = colorMap;

void RhythmCore::setup(CoreParams params) {
	Core::setup(params);

	songIndex = 0;
}

void RhythmCore::drawLights(unsigned char peakCount) {
	int middleLEDCount = (LED_COUNT / 2.0);
	int leftLimit = middleLEDCount - (peakCount / 2.0);
	int rightLimit = middleLEDCount + (peakCount / 2.0);

	for (int f = 0; f <= LED_COUNT; f++) {
		if (f > leftLimit && f < rightLimit) {
			unsigned char paletteIndex = (255 * f) / middleLEDCount;

			if (f > middleLEDCount) {
				paletteIndex = 255 - paletteIndex;
			}

			leds[f] = ColorFromPalette(palette, paletteIndex);
		}
		else {
			leds[f] = CRGB::Black;
		}
	}
}

void RhythmCore::playBackgroundTrack() {
	if (!bkgTrack->isPlaying()) {
		playTrack("beat", song[songIndex % songLength]);
		songIndex++;
	}
}
