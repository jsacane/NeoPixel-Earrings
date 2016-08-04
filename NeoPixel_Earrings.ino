#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUM_PIXELS 16
#define RING_PIN 0
#define MIC_PIN 1

int animation = 0; // tracks current animation
const long duration = 15000; // 15 seconds in milliseconds

Adafruit_NeoPixel ring = Adafruit_NeoPixel(NUM_PIXELS, RING_PIN);

void setup()
{  
  ring.begin();
  ring.setBrightness(63);
  ring.show();
}

void loop()
{
  unsigned long start = millis(); // reset timer
  resetRing();
  
  while (millis() - start < duration) {
    // switch animations every 30 seconds
    switch(animation) {
      case 0:
        colorWipe(ring.Color(255, 0, 0), 50); // Red
        colorWipe(ring.Color(0, 255, 0), 50); // Green
        colorWipe(ring.Color(0, 0, 255), 50); // Blue
        break;

      case 1:
        rainbow(30);
        break;

      case 2:
        chase(40);
        break;

      case 3:
        rainbowCycle(30);
        break;

      case 4:
        theaterChaseRainbow(60);
        break;
    }
  }
  
  animation = (animation + 1) % 5;
}

// Wipe through red/green/blue
void colorWipe(uint32_t color, uint8_t wait)
{
  for (uint16_t i=0; i<ring.numPixels(); i++) {   
    ring.setPixelColor(i, color);
    ring.show();
    delay(wait);
  }
}

// Smoothly cycle through all colors
void rainbow(uint8_t wait)
{
  uint16_t i, j;
  
  for (j=0; j<256; j++) {
    
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
