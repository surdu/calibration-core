#include "Core.h"



Core::Core(AudioPlaySdWav* bkgTrack, AudioPlaySdWav* talkTrack) {
	this->bkgTrack = bkgTrack;
	this->talkTrack = talkTrack;

	tracksCountMap = new SimpleMap<String, int>([](String &a, String &b) -> int {
		if (a == b) {
			return 0;
		}
		else if (a > b) {
			return 1;
		}
		else {
			return -1;
		}
	});
};

void Core::setName(char* coreName) {
	this->coreName = coreName;
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
	char cacheKey[16] = "";
	strcat(cacheKey, coreName);
	strcat(cacheKey, trackName);

	int cached = tracksCountMap->get(cacheKey);
	if (cached) {
		return cached;
	}

	int tracksCount = 1;

	char filename[22] = "";
	buildFilename(filename, trackName, tracksCount);

	while(SD.exists(filename)) {
		tracksCount++;
		buildFilename(filename, trackName, tracksCount);
	}

	tracksCountMap->put(cacheKey, tracksCount - 1);

	Serial.print("Resolved: ");
	Serial.println(tracksCount - 1);

	return tracksCount - 1;
}

void Core::play() {
	int effectTracksCount = getTracksCount("effect");
	int talkTracksCount = getTracksCount("talk");
	int nagTracksCount = getTracksCount("nag");

	if (effectTracksCount > 0) {
		bool startedTrack = playTrack("effect", nextEffectsTrack);
		if (startedTrack) {
			if (nextEffectsTrack < effectTracksCount) {
				nextEffectsTrack++;
			}
			else {
				nextEffectsTrack = 1;
			}
		}
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
