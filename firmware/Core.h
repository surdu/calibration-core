#ifndef Core_h
#define Core_h

#include <Audio.h>
#include <Arduino.h>
#include <FastLED.h>
#include "src/millisDelay.h"

#define LED_COUNT 64
#define LED_DATA_PIN 3

struct CoreParams {
    char *name;
    int color;
};

enum State {
	Talking,
	Nagging
};

class Core {
	public:
		CRGB* leds;
		CoreParams params;
		AudioPlaySdWav* bkgTrack;

		Core(AudioPlaySdWav* bkgTrack, AudioPlaySdWav* talkTrack, AudioAnalyzePeak* peak, CRGB* leds);
		void setup(CoreParams desc);
		bool playTrack(char* trackName, unsigned char trackNumber);
		virtual void drawLights(unsigned char peakCount);
		virtual void playBackgroundTrack();
		void loop();

	private:
		AudioPlaySdWav* talkTrack;
		AudioAnalyzePeak* peak;

		unsigned char effectsTrack;
		unsigned char currentTalkTrack;
		State currentState;

		unsigned char effectTracksCount;
		unsigned char talkTracksCount;
		unsigned char nagTracksCount;

		millisDelay talkTimer;

		elapsedMillis fps;

		void buildFilename(char* result, char* trackName, unsigned char trackNumber);
		unsigned char getTracksCount(char* trackName);
		void moveToNextTalkTrack();
		void lightsLoop();
};

#endif
