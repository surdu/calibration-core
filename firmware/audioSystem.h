#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           bkgTrack;       //xy=250,116
AudioPlaySdWav           talkTrack;      //xy=250,171
AudioMixer4              mixer1;         //xy=475,125
AudioAnalyzePeak         talkPeak;          //xy=476,268
AudioAnalyzePeak         effectsPeak;          //xy=484,211
AudioOutputI2S           i2s1;           //xy=669,130
AudioConnection          patchCord1(bkgTrack, 0, mixer1, 0);
AudioConnection          patchCord2(bkgTrack, 0, effectsPeak, 0);
AudioConnection          patchCord3(talkTrack, 0, mixer1, 1);
AudioConnection          patchCord4(talkTrack, 0, talkPeak, 0);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=678,210
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
