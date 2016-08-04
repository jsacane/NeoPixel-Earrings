#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUM_PIXELS 16
#define RING_PIN 0
#define MIC_PIN 1

int animation = 0; // tracks current animation
const long duration = 20000; // duration of each animation
unsigned long start = millis(); // initialize global timer

Adafruit_NeoPixel ring = Adafruit_NeoPixel(NUM_PIXELS, RING_PIN);

void setup()
{  
  ring.begin();
  ring.setBrightness(63);
  ring.show();
}

void loop()
{  
  // change animation every 20 seconds
  switch(animation) {
    case 0:
      rgbWipe(50);
      break;

    case 1:
      rainbow(30);
      break;

    case 2:
      chase(50);
      break;

    case 3:
      rainbowCycle(30);
      break;

    case 4:
      theaterChaseRainbow(60);
      break;
  }
}

// Wipe through red/green/blue colors
void rgbWipe(uint8_t wait)
{
  uint16_t i, j;
  uint32_t colors[3] = { ring.Color(255,0,0), ring.Color(0,255,0), ring.Color(0,0,255) };
  
  for (j=0; j<3; j++) {

    if (millis() - start >= duration) { // change animation every 15 seconds
      animation = (animation + 1) % 5;
      start = millis(); // reset timer
      resetRing();
      break;
    }
    
    for (i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, colors[j]);
      ring.show();
      delay(wait);
    }
  }
}

// Smoothly cycle through all colors
void rainbow(uint8_t wait)
{
  uint16_t i, j;
  
  for (j=0; j<256; j++) {

    if (millis() - start >= duration) {
      animation = (animation + 1) % 5;
      start = millis();
      resetRing();
      break;
    }
    
    for (i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, Wheel((i+j) & 255));
    }
    ring.show();
    delay(wait);
  }
}

// Two white pixels chase each other around the ring
void chase(uint8_t wait)
{  
  // initialize first two pixels
  ring.setPixelColor(0, ring.Color(255,255,255));
  ring.setPixelColor(8, ring.Color(255,255,255));
  ring.show();
  
  for (uint16_t i=0; i<ring.numPixels(); i++) {

    if (millis() - start >= duration) {
      animation = (animation + 1) % 5;
      start = millis();
      resetRing();
      break;
    }
    
    // turn on next pixels
    ring.setPixelColor(((i+1) % 16), ring.Color(255,255,255));
    ring.setPixelColor(((i+9) % 16), ring.Color(255,255,255));
    // turn off previous pixels
    ring.setPixelColor((i % 16), ring.Color(0,0,0));
    ring.setPixelColor(((i+8) % 16), ring.Color(0,0,0));
    
    ring.show();
    delay(wait);
  }  
}

// Rainbow w/ all colors at once
void rainbowCycle(uint8_t wait)
{
  uint16_t i, j;

  for (j=0; j<256*5; j++) {

    if (millis() - start >= duration) {
      animation = (animation + 1) % 5;
      start = millis();
      resetRing();
      break;
    }
    
    for (i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, Wheel(((i * 256 / ring.numPixels()) + j) & 255));
    }
    ring.show();
    delay(wait); 
  }
}

// Theater-style chase w/ all colors
void theaterChaseRainbow(uint8_t wait)
{  
  for (int j=0; j<256; j++) { // cycle through all 256 colors

    if (millis() - start >= duration) {
      animation = (animation + 1) % 5;
      start = millis();
      resetRing();
      break;
    }
      
    for (int q=0; q<3; q++) {
      
      for (uint16_t i=0; i<ring.numPixels(); i+=3) { // turn on every third pixel
        ring.setPixelColor(i+q, Wheel((i+j) % 255));
      }

      ring.show();
      delay(wait);

      for (uint16_t i=0; i<ring.numPixels(); i+=3) { // turn off every third pixel
        ring.setPixelColor(i+q, 0);
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return ring.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return ring.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return ring.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Resets the ring to be off
void resetRing()
{
  for (uint16_t i=0; i<ring.numPixels(); i++) {
    ring.setPixelColor(i, 0);
    ring.show();
  }
}
