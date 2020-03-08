#include "audioSystem.h"
#include "Core.h"

#include "cores.h"

Core* core;
CoreParams chosenCore;

void setup() {
  Serial.begin(9600);
	randomSeed(analogRead(2));

  setupAudioSystem();

  chosenCore = cores[random(0, coresCount)];

  core = new Core(&bkgTrack, &talkTrack, &peak);
  core->set(chosenCore);
}

void loop() {
  core->loop();
}
