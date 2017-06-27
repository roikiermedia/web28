#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "FastLED.h"

#define NETWORK_NAME "YOUR SSID"
#define NETWORK_PASSWORD "YOUR PASSWORD"

#define DATA_PIN    6
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    170
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         100
#define FRAMES_PER_SECOND  60

ESP8266WebServer server(80);

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current


void returnOK() {
  server.send(200, "text/plain", "");
}

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

  if (MDNS.begin("web28")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", [](){
    FastLED.showColor(0xFFFFFF);
    server.send(200, "text/html", "Hello World!");
  });
  server.on("/preset/0", [](){
    gCurrentPatternNumber = 0;
    server.send(200, "text/html", "Preset 0");
  });
  server.on("/preset/1", [](){
    gCurrentPatternNumber = 1;
    server.send(200, "text/html", "Preset 1");
  });
  server.on("/preset/2", [](){
    gCurrentPatternNumber = 2;
    server.send(200, "text/html", "Preset 2");
  });
  server.on("/preset/3", [](){
    gCurrentPatternNumber = 3;
    server.send(200, "text/html", "Preset 3");
  });
  server.on("/preset/4", [](){
    gCurrentPatternNumber = 4;
    server.send(200, "text/html", "Preset 4");
  });
  server.on("/preset/5", [](){
    gCurrentPatternNumber = 5;
    server.send(200, "text/html", "Preset 5");
  });
  server.begin();

  for (int i=0; i<=NUM_LEDS; i++) {
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

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop() {
  server.handleClient();
  
  gPatterns[gCurrentPatternNumber]();
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

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
