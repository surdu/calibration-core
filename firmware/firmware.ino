#include "audioSystem.h"
#include <FastLED.h>
#include "Core.h"

#include "cores.h"

#define NUM_LEDS 64
#define DATA_PIN 17

#define LED_COUNT 64

Core* core;
CoreDescription chosenCore;

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
	randomSeed(analogRead(2));

  setupAudioSystem();

  chosenCore = cores[random(0, coresCount)];

  core = new Core(&bkgTrack, &talkTrack);
  core->setName(chosenCore.name);

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

elapsedMillis fps;

void loop() {
  core->loop();

  if (fps > 24) {
    if (peak.available()) {
      fps = 0;
      int peakCount = peak.read() * LED_COUNT;

      if (peakCount % 2 != 0) {
        peakCount += 1;
      }

      int leftLimit = (LED_COUNT / 2.0) - (peakCount / 2.0);
      int rightLimit = (LED_COUNT / 2.0) + (peakCount / 2.0);

      for (int f = 0; f <= LED_COUNT; f++) {
        if (f > leftLimit && f < rightLimit) {
          leds[f] = chosenCore.color;
        }
        else {
          leds[f] = CRGB::Black;
        }

      }
      FastLED.setBrightness(2);
      FastLED.show();
    }
  }
}
