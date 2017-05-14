#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "FastLED.h"

#define NETWORK_NAME "Chaostreff-Flensburg"
#define NETWORK_PASSWORD "Schnell33"

#define DATA_PIN    6
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    170
CRGB leds[NUM_LEDS];

#define BRIGHTNESS         100
#define FRAMES_PER_SECOND  60

ESP8266WebServer server(80);


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
    FastLED.showColor(0x0000FF);
    server.send(200, "text/html", "Hello World!");
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

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop() {
  server.handleClient();
  
  //confetti();
  //FastLED.show();
  //FastLED.delay(1000/FRAMES_PER_SECOND);
  //EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
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
