#ifndef Core_h
#define Core_h

#include <Audio.h>
#include <Arduino.h>
#include "src/millisDelay.h"

struct CoreDescription {
    char *name;
    int color;
};

enum State {
	Talking,
	Nagging
};

class Core {
	public:
		Core(AudioPlaySdWav* bkgTrack, AudioPlaySdWav* talkTrack);
		void setName(char* coreName);
		bool playTrack(char* trackName, unsigned char trackNumber);
		void loop();

	private:
		char *coreName;
		AudioPlaySdWav* bkgTrack;
		AudioPlaySdWav* talkTrack;

		unsigned char effectsTrack;
		unsigned char currentTalkTrack;
		State currentState;

		unsigned char effectTracksCount;
		unsigned char talkTracksCount;
		unsigned char nagTracksCount;

		millisDelay talkTimer;

		void buildFilename(char* result, char* trackName, unsigned char trackNumber);
		unsigned char getTracksCount(char* trackName);
		void Core::moveToNextTalkTrack();
};

#endif
