#include "Core.h"
#include "audioSystem.h"
#include <FastLED.h>

#define NUM_LEDS 64
#define DATA_PIN 17

#define LED_COUNT 64.0;

Core* core;

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);

  setupAudioSystem();

  core = new Core(&bkgTrack, &talkTrack);
  core->setName("boring");

  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

elapsedMillis fps;

void loop() {
  core->loop();

  if (fps > 24) {
    if (peak.available()) {
      fps = 0;
      int peakCount = peak.read() * LED_COUNT;

      for (int f = 0; f <= 64; f++) {
        if (f < peakCount) {
          leds[f].setRGB(10, 0, 0);
        }
        else {
          leds[f] = CRGB::Black;
        }

      }
      FastLED.show();
    }
  }
}
