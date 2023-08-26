#include "FastLED.h"

#define NUM_LEDS 178
#define LED_PIN 5
CRGB leds[NUM_LEDS];

void ledSetup() {
   FastLED.addLeds< WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
}

void ledTick() {
 Preset preset = getCurrentPreset();
 FastLED.setBrightness(preset.brightness);


  for (int i = 0; i < NUM_LEDS; i++ ) {         // от 0 до первой трети
     if(data.isLedEnabled) {
      leds[i] = CHSV(preset.hue, preset.saturation, preset.brightness);
     } else {
      leds[i] = CRGB::Black;
     }
  }
  FastLED.show();
}
