#include <ESP8266WiFi.h>
#include "FastLED.h"

#define NETWORK_NAME ""
#define NETWORK_PASSWORD ""

#define DATA_PIN    6
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    170
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         100
#define FRAMES_PER_SECOND  60


void setup() {
  Serial.begin(115200);
  Serial.println();

  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  
  WiFi.begin(NETWORK_NAME, NETWORK_PASSWORD);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    FastLED.delay(1000/FRAMES_PER_SECOND);
    Serial.print(".");
    runner(0xFF0000);
    FastLED.show();  
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  for (int i=0; i<=NUM_LEDS*1.5; i++) {
    runner(0x00FF00);
    FastLED.show();
    FastLED.delay(1000/FRAMES_PER_SECOND);
  }
  for (int i=0; i<=50; i++) {
    runner(0x000000);
    FastLED.show();
    FastLED.delay(1000/FRAMES_PER_SECOND/4);
  }
}

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop() {
  confetti();
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
}

void runner(CRGB clr)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += clr;
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}
