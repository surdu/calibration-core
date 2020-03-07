#include "Core.h"

#define TALK_PAUSE 2000
#define NAG_PAUSE 3000

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

	currentTalkTrack = 1;
	currentState = Talking;

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

void Core::moveToNextTalkTrack() {
	unsigned char trackLimit;

	switch (currentState)
	{
		case Talking:
			trackLimit = talkTracksCount;
			break;
		case Nagging:
			trackLimit = nagTracksCount;
			break;
	}

	if (currentTalkTrack < trackLimit) {
		currentTalkTrack++;
	}
	else {
		currentTalkTrack = 1;
		currentState = Nagging;
	}
}

void Core::loop() {
	if (effectTracksCount > 0) {
		playTrack("effect", effectsTrack);
	}

	if (!talkTrack->isPlaying()) {
		char* talkTrack;
		unsigned int pause;

		switch (currentState)
		{
			case Talking:
				talkTrack = "talk";
				if (currentTalkTrack == 1) {
					pause = 0;
				}
				else {
					pause = TALK_PAUSE;
				}
				break;
			case Nagging:
				talkTrack = "nag";
				pause = NAG_PAUSE;
				break;
		}

		if (!talkTimer.isRunning()) {
			talkTimer.start(pause);
		}

		if (talkTimer.isFinished()) {
			playTrack(talkTrack, currentTalkTrack);
			moveToNextTalkTrack();
		}
	}

}
