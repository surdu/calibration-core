#include "Core.h"

#define TALK_PAUSE 2000
#define NAG_PAUSE 5000

Core::Core(AudioPlaySdWav* bkgTrack, AudioPlaySdWav* talkTrack) {
	this->bkgTrack = bkgTrack;
	this->talkTrack = talkTrack;

	randomSeed(analogRead(2));
};

void Core::setName(char* coreName) {
	this->coreName = coreName;

	effectTracksCount = getTracksCount("effect");
	talkTracksCount = getTracksCount("talk");
	nagTracksCount = getTracksCount("nag");

	nextTalkTrack = 1;
	nextNagTrack = 1;

	currentTalkTrack = 1;
	currentNagTrack = 1;

	if (effectTracksCount > 0) {
		effectsTrack = 1;
		if (effectTracksCount > 1) {
			effectsTrack = random(1, effectTracksCount + 1);
		}
	}
	else {
		effectsTrack = 0;
	}
}

void Core::buildFilename(char* result, char* trackName, unsigned char trackNumber) {
	strcpy(result, "");
	strcat(result, coreName);
	strcat(result, "/");
	strcat(result, trackName);
	strcat(result, "_");
	sprintf(result + strlen(result), "%d", trackNumber);
	strcat(result, ".wav");
}

bool Core::playTrack(char* trackName, unsigned char trackNumber) {
	if (trackNumber == 0) {
		return false;
	}

	AudioPlaySdWav* track;

	if (strcmp(trackName, "effect") == 0) {
		track = bkgTrack;
	}
	else {
		track = talkTrack;
	}

	if (!track->isPlaying()) {
		char filename[22] = "";
		buildFilename(filename, trackName, trackNumber);
		track->play(filename);
		delay(10);
		return true;
	}

	return false;
}

unsigned char Core::getTracksCount(char* trackName) {
	unsigned char tracksCount = 1;

	char filename[22] = "";
	buildFilename(filename, trackName, tracksCount);

	while(SD.exists(filename)) {
		tracksCount++;
		buildFilename(filename, trackName, tracksCount);
	}

	return tracksCount - 1;
}

void Core::playNextTalkTrack() {
	bool startedTrack = playTrack("talk", nextTalkTrack);
	if (startedTrack) {
		if (nextTalkTrack < talkTracksCount) {
			nextTalkTrack++;
		}
		else {
			nextTalkTrack = 0;
		}
	}
}

void Core::playNag() {
		bool startedTrack = playTrack("nag", nextNagTrack);
	if (startedTrack) {
		if (nextNagTrack < nagTracksCount) {
			nextNagTrack++;
		}
		else {
			nextNagTrack = 1;
		}
	}
}

void Core::play() {
	if (effectTracksCount > 0) {
		playTrack("effect", effectsTrack);
	}

	if (!talkTrack->isPlaying()) {
		if (currentTalkTrack != nextTalkTrack) {
			currentTalkTrack = nextTalkTrack;
			talkTimer.start(TALK_PAUSE);
		}
		else if (!talkTimer.isRunning()) {
			playNextTalkTrack();
		}

		if (talkTimer.isFinished()) {
			playNextTalkTrack();
		}

		if (nextTalkTrack == 0) {
			if (currentNagTrack != nextNagTrack) {
				currentNagTrack = nextNagTrack;
				talkTimer.start(NAG_PAUSE);
			}
			else if (!talkTimer.isRunning()) {
				playNag();
			}

			if (talkTimer.isFinished()) {
				playNag();
			}
		}
	}

}
