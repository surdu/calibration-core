#include "Core.h"


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

void Core::buildFilename(char* result, char* trackName, int trackNumber) {
	strcpy(result, "");
	strcat(result, coreName);
	strcat(result, "/");
	strcat(result, trackName);
	strcat(result, "_");
	sprintf(result + strlen(result), "%d", trackNumber);
	strcat(result, ".wav");
}

bool Core::playTrack(char* trackName, int trackNumber) {
	if (trackNumber == 0) {
		return false;
	}

	AudioPlaySdWav* track;

	if (strcmp(trackName, "talk") == 0) {
		track = talkTrack;
	}
	else {
		track = bkgTrack;
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

int Core::getTracksCount(char* trackName) {
	int tracksCount = 1;

	char filename[22] = "";
	buildFilename(filename, trackName, tracksCount);

	while(SD.exists(filename)) {
		tracksCount++;
		buildFilename(filename, trackName, tracksCount);
	}

	return tracksCount - 1;
}

void Core::play() {
	if (effectTracksCount > 0) {
		playTrack("effect", effectsTrack);
	}

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
