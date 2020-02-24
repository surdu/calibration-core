#include "Core.h"
#include "audioSystem.h"

Core* core;

void setup() {
  Serial.begin(9600);

  setupAudioSystem();

  core = new Core(&bkgTrack, &talkTrack);
  core->setName("boring");
}


void loop() {
  core->play();
}
