#ifndef Core_h
#define Core_h

#include <Audio.h>
#include <Arduino.h>

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
		int effectsTrack;

		int nextTalkTrack;

		int effectTracksCount;
		int talkTracksCount;
		int nagTracksCount;

		void buildFilename(char* result, char* trackName, int trackNumber);
		int getTracksCount(char* trackName);
};

#endif
