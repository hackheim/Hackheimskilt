#include <FastLED.h>

#define LED_PIN  3

#define COLOR_ORDER GRB
#define CHIPSET     NEOPIXEL
#define NUM_STRIPS 8
#define NUM_LEDS_PER_STRIP 46
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

#define BRIGHTNESS 64

#define HWSERIAL Serial1

unsigned long currentTime = 0;
long prevTime = 0;
long hueTime = 0;
long blurTime = 0;

float pixelDelay = 250;
float hueDelay = pixelDelay * 20;
float blurDelay = pixelDelay * 5;

uint8_t hue = 0;
uint8_t lum = 0;
uint8_t x = 0;
uint8_t y = 0;

// Helper functions for an two-dimensional XY matrix of pixels.
// Simple 2-D demo code is included as well.
//
//     XY(x,y) takes x and y coordinates and returns an LED index number,
//             for use like this:  leds[ XY(x,y) ] == CRGB::Red;
//             No error checking is performed on the ranges of x and y.
//
//     XYsafe(x,y) takes x and y coordinates and returns an LED index number,
//             for use like this:  leds[ XY(x,y) ] == CRGB::Red;
//             Error checking IS performed on the ranges of x and y, and an
//             index of "-1" is returned.  Special instructions below
//             explain how to use this without having to do your own error
//             checking every time you use this function.
//             This is a slightly more advanced technique, and
//             it REQUIRES SPECIAL ADDITIONAL setup, described below.


// Params for width and height
const uint8_t kMatrixWidth = NUM_LEDS_PER_STRIP;
const uint8_t kMatrixHeight = NUM_STRIPS;

// Param for different pixel layouts
const bool    kMatrixSerpentineLayout = false;

int n = 0;

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;

  if( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }

  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }

  return i;
}

//this function doesn't do anything right now AFAIK, just there from the example, might remove if I figure out it does nothing

void DrawOneFrame( byte startHue8, int8_t yHueDelta8, int8_t xHueDelta8)
{
  byte lineStartHue = startHue8;
  for( byte y = 0; y < kMatrixHeight; y++) {
    lineStartHue += yHueDelta8;
    byte pixelHue = lineStartHue;
    for( byte x = 0; x < kMatrixWidth; x++) {
      pixelHue += xHueDelta8;
      leds[ XY(x, y)]  = CHSV( pixelHue, 255, 255);
    }
  }
}

void randomfade() {

  currentTime = millis();

  //changes the hue with a value between 0 and 3 every set interval
  if(currentTime-hueTime > hueDelay){

    hue = hue + random(0, 3);
    hueTime = currentTime;
    FastLED.show();
  }

  //blurs the whole array every set interval
  if(currentTime-blurTime > blurDelay){

    blur2d( leds, kMatrixWidth, kMatrixHeight, 3);
    blurTime = currentTime;
    FastLED.show();
  }

  //lights up another random pixel every set interval
  if(currentTime-prevTime > pixelDelay){

    uint8_t saturation = random(170,255);
    leds[XY(random(0,42),random(0,8))] = CHSV(hue, saturation, 255);
    FastLED.show();

    prevTime = currentTime;

  }
}

void loop(){

  randomfade();


}

void setup() {
  //declares the array of LEDS to the leds[] array
  FastLED.addLeds<WS2812B, 5>  (leds, 0, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B, 6>  (leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B, 7>  (leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B, 8>  (leds, 3 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B, 9>  (leds, 4 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B, 10>  (leds, 5 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B, 11> (leds, 6 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812B, 12> (leds, 7 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.setBrightness( BRIGHTNESS );

  Serial.begin(9600);

  //seeds the pseudorandom generator with some noise from a floating analog port
  randomSeed(analogRead(A0));

  //initiates the sign to run at a randomly selected hue each time it starts
  hue = random(0,255);

  //fills the array with a randomly generated pattern when it starts and lets it run for 2000ms like this
  for(int i = 0; i < 200; i++){
    uint8_t saturation = random(170,255);
    leds[XY(random(0,42),random(0,8))] = CHSV(hue, saturation, 255);
  }
  blur2d( leds, kMatrixWidth, kMatrixHeight, 10);
  FastLED.show();
  delay(2000);

}
