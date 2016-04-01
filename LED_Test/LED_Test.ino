#include <Wire.h>
#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET
 

#define NUMPIXELS 48 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
// Arduino Mega
#define DATAPIN    4
#define CLOCKPIN   5
// Teensy Breakout
//#define DATAPIN  16
//#define CLOCKPIN 17
Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS, DATAPIN, CLOCKPIN);

const int ledPin = 13;
volatile int control = 0;
volatile int wait = 25;

void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  Wire.begin(5);
  Wire.onReceive(receiveEvent);

  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  pinMode(ledPin,OUTPUT);
  Serial.begin(115200);
}


// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

int      head  = 0, tail = -4; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)
int i= 0,j=0,x=0;
byte red = 0, green = 0, blue = 0;

void loop() {
  switch (control) {
    case 1:
      angle(wait);
      break;
    case 2:
      theaterChaseRainbow(wait, 2);
      break;
    case 3:
      theaterChaseRainbow(wait, 1);
      break;
    default:
      noCommand();
      break;
  }  
}

void noCommand() {
  for (int c = 0; c < strip.numPixels(); c++) {
    strip.setPixelColor(c, 0, 255, 0);
    strip.show();
    delay(wait);
    strip.setPixelColor(c, 0);
    strip.show();
    strip.setPixelColor(c-2, 50, 150, 50);
    strip.show();
    delay(wait);
    strip.setPixelColor(c-2, 0);
    strip.show();
    strip.setPixelColor(c-4, 50, 50, 50);
    strip.show();
    delay(wait);
    strip.setPixelColor(c-4, 0);
    strip.show();
  }
}

void angle(int angle) {
  int number = int(float(angle) * (48.0 / 90.0));
  for (int c = 0; c < number; c++) {
    strip.setPixelColor(c, 255, 0, 0);
  }
  strip.show();
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait, uint8_t dir) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    if (dir == 1) {
      for (int q=0; q < 8; q++) {
        for (int i=0; i < strip.numPixels(); i=i+8) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+8) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
      }
    }
    else if (dir == 2) {
      for (int q=8; q > 0; q--) {
        for (int i=0; i < strip.numPixels(); i=i+8) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+8) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
      }
    }
    
    digitalWrite(ledPin,!digitalRead(ledPin));
  }
}

void receiveEvent(int howMany) {
  while (Wire.available()) {
    int control = Wire.read();
    int wait = Wire.read();
    }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i*2+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}
