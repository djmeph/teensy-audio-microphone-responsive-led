/* Mono Peak Meter
   Scrolling peak audio level meter in the Arduino Serial Monitor
   Audio input needs to connect to pin 16 (A2).  The signal range is 0 to 1.2V.
   See the documentation in the Audio System Design Tool for the recommended
   circuit to connect an analog signal.
   This example code is in the public domain
*/

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <FastLED.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s1;           //xy=138,113
AudioAnalyzePeak         peak1;          //xy=404,184
AudioConnection          patchCord1(i2s1, 0, peak1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=143,199
// GUItool: end automatically generated code

// LED Setup
#define LED_PIN 17
#define NUM_LEDS 45
CRGB leds[NUM_LEDS];


void setup() {
  AudioMemory(8);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(36);
  delay(1000);
}

// for best effect make your terminal/monitor a minimum of 31 chars wide and as high as you can.

elapsedMillis fps;
float monoPeak = 0;
int peakValue = 0;
int i = 0;

void loop() {
  if (fps > 24) {
    /*
    for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(255,0,0);
      FastLED.show();
    }
    */
    if (fps > 24) {
      if (peak1.available()) {
        fps = 0;
        monoPeak = peak1.read();
        peakValue = monoPeak * NUM_LEDS;
        for (i = 0; i < peakValue; i++) {
          leds[i] = CRGB(148,0,211);
          FastLED.show();
        }
        for (i = NUM_LEDS - peakValue; i < NUM_LEDS; i++) {
          leds[i] = CRGB(0,0,255);
          FastLED.show();
        }
      }
    }
  }
}
