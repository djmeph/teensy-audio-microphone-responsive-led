#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <FastLED.h>

AudioInputI2S i2s1;
AudioAnalyzePeak peak1;
AudioConnection patchCord1(i2s1, 0, peak1, 0);
AudioControlSGTL5000 sgtl5000_1;

#define LED_PIN 17
#define NUM_LEDS 45
#define ANALOG_IN A1
CRGB leds[NUM_LEDS];

void setup() {
  AudioMemory(8);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_MIC);
  sgtl5000_1.micGain(0);
  delay(1000);
}

elapsedMillis fps;
int peakValue = 0;
int i = 0;
int gainValue = 0;
int newGain = 0;

void loop() {
  if (fps > 24) {
    if (peak1.available()) {
      fps = 0;
      peakValue = peak1.read() * NUM_LEDS;
      for (i = 0; i < peakValue; i++) {
        leds[i] = CRGB(148,0,211);
        FastLED.show();
      }
      for (i = NUM_LEDS - peakValue; i < NUM_LEDS; i++) {
        leds[i] = CRGB(0,0,255);
        FastLED.show();
      }
      newGain = analogRead(ANALOG_IN);
      if (newGain != gainValue) {
        gainValue = newGain;
        sgtl5000_1.micGain(map(gainValue, 0, 1023, 0, 63));
      }
    }
  }
}
