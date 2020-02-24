#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           bkgTrack;     //xy=229,116
AudioPlaySdWav           talkTrack;     //xy=229,171
AudioMixer4              mixer1;         //xy=454,125
AudioOutputI2S           i2s1;           //xy=648,130
AudioConnection          patchCord1(bkgTrack, 0, mixer1, 0);
AudioConnection          patchCord2(talkTrack, 0, mixer1, 1);
AudioConnection          patchCord3(mixer1, 0, i2s1, 0);
AudioConnection          patchCord4(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=318,366
// GUItool: end automatically generated code


void setupAudioSystem() {
	#define SDCARD_CS_PIN    10
	#define SDCARD_MOSI_PIN  7
	#define SDCARD_SCK_PIN   14

	AudioMemory(8);

	sgtl5000_1.enable();
	sgtl5000_1.volume(0.45);

	SPI.setMOSI(SDCARD_MOSI_PIN);
	SPI.setSCK(SDCARD_SCK_PIN);
	if (!(SD.begin(SDCARD_CS_PIN))) {
		while (1) {
			Serial.println("Unable to access the SD card");
			delay(500);
		}
	}

	mixer1.gain(0, 0.25);
	mixer1.gain(1, 0.5);

	delay(1000);
}
