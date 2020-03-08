#include "audioSystem.h"

#include "Core.h"
#include "RhythmCore.h"

#include "cores.h"

Core* core;
CoreParams chosenCore;
CRGB leds[LED_COUNT];

void setup() {
  Serial.begin(9600);
	randomSeed(analogRead(2));

  setupAudioSystem();

  chosenCore = cores[random(0, coresCount)];

  core = new Core(&bkgTrack, &talkTrack, &peak, leds);
  core->setup(chosenCore);
}

void loop() {
  core->loop();
}
