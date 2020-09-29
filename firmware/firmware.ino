#include "audioSystem.h"

#include "Core.h"
#include "RhythmCore.h"

#include "cores.h"

#include "MPU.h"

Core* core;
CoreParams chosenCore;
CRGB leds[LED_COUNT];

void setup() {
  Serial.begin(9600);
	randomSeed(analogRead(2));

  setupAudioSystem();

  chosenCore = cores[random(0, coresCount)];

  if (strcmp(chosenCore.name, "rhythm") == 0) {
    core = new RhythmCore(&bkgTrack, &talkTrack, &effectsPeak, leds);
  }
  else {
    core = new Core(&bkgTrack, &talkTrack, &talkPeak, leds);
  }

  core->setup(chosenCore);
  setupMPU(triggered);
}

void triggered() {
  Serial.println("Triggered");
}

void loop() {
  core->loop();
  MPUloop();
}
