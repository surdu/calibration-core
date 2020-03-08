#ifndef Core_h
#define Core_h

#include <Audio.h>
#include <Arduino.h>
#include <FastLED.h>
#include "src/millisDelay.h"

#define LED_COUNT 64
#define LED_DATA_PIN 17

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

		Core(AudioPlaySdWav* bkgTrack, AudioPlaySdWav* talkTrack, AudioAnalyzePeak* peak, CRGB* leds);
		void set(CoreParams desc);
		bool playTrack(char* trackName, unsigned char trackNumber);
		virtual void drawLights(unsigned char peakCount);
		void loop();

	private:
		AudioPlaySdWav* bkgTrack;
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
		void playBackgroundTrack();
		void lightsLoop();
};

#endif
