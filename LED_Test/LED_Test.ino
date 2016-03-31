// Simple strand test for Adafruit Dot Star RGB LED strip.
// This is a basic diagnostic tool, NOT a graphics demo...helps confirm
// correct wiring and tests each pixel's ability to display red, green
// and blue and to forward data down the line.  By limiting the number
// and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!

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


// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
//Adafruit_DotStar strip = Adafruit_DotStar(NUMPIXELS);

const int ledPin = 13;


void setup() {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

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
  theaterChaseRainbow(25);
  
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
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
    digitalWrite(ledPin,!digitalRead(ledPin));
  }
}

void followrainbow(uint8_t wait){
  // Now Broken!!!
  uint16_t x,j,c;
  c=0;
  for(j = 0; j<256;j++){
    for(x = 0; x < strip.numPixels(); x++){
      if(x <=head && x>tail){
        strip.setPixelColor(x,Wheel(j+x*2));
      }else{
        strip.setPixelColor(x,0);
      }
      c++;
      if( c>10){
        c=0;
        head+=1;
        tail+=1;
        if(head > strip.numPixels()){
          head=0;
          tail=-4;
        }
      }
      
    }
    strip.show();
    delay(wait);
    digitalWrite(ledPin,!digitalRead(ledPin));
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
