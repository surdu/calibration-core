#ifndef Core_h
#define Core_h

#include <Audio.h>
#include <Arduino.h>

#include "SimpleMap.h"

class Core {
	public:
		char *coreName;
		AudioPlaySdWav* bkgTrack;
		AudioPlaySdWav* talkTrack;

		Core(AudioPlaySdWav* bkgTrack, AudioPlaySdWav* talkTrack);
		void setName(char* coreName);
		bool playTrack(char* trackName, int trackNumber);
		void play();

	private:
		int nextEffectsTrack = 1;
		int nextTalkTrack = 1;

		SimpleMap<String, int> *tracksCountMap;

		void buildFilename(char* result, char* trackName, int trackNumber);
		int getTracksCount(char* trackName);
};

#endif
