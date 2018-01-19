#include <FastLED.h>

#define NUM_LEDS_PER_STRIP 139
#define NUM_STRIPS 12
#define BRIGHTNESS  20

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

// Pin layouts on the teensy 3/3.1:
// WS2811_PORTD: 2,14,7,8,6,20,21,5
// WS2811_PORTC: 15,22,23,9,10,13,11,12,28,27,29,30 (these last 4 are pads on the bottom of the teensy)
// WS2811_PORTDC: 2,14,7,8,6,20,21,5,15,22,23,9,10,13,11,12 - 16 way parallel

/*
 * A=0; B=0; C = 0 -> Lower LED Panel
 * A=1; B=0; C = 0 -> Middle LED Panel
 * A=0; B=1; C = 0 -> Upper LED Panel
 */
int pinA = 20;
int pinB = 21;
int pinC = 17;

void setup() {

  delay(3000);
  
  LEDS.addLeds<WS2811_PORTC,NUM_STRIPS>(leds, NUM_LEDS_PER_STRIP);
  LEDS.setBrightness(BRIGHTNESS);

  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);

  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);
}

void hue_show() {
  static uint8_t hue = 0;
  for(int i = 0; i < NUM_STRIPS; i++) {
    for(int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
      leds[(i*NUM_LEDS_PER_STRIP) + j] = CHSV((32*i) + hue+j,192,255);
    }
  }

  // Set the first n leds on each strip to show which strip it is
  for(int i = 0; i < NUM_STRIPS; i++) {
    for(int j = 0; j <= i; j++) {
      leds[(i*NUM_LEDS_PER_STRIP) + j] = CRGB::Red;
    }
  }
  hue++;
  LEDS.show();
  LEDS.delay(10);
}


#define SPARKING 120
#define COOLING  55
#define FRAMES_PER_SECOND 60
bool gReverseDirection = false;

void Fire2012()
{
  
// Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS_PER_STRIP];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS_PER_STRIP) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k = NUM_LEDS_PER_STRIP - 1; k >= 2; k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
      CRGB color = HeatColor( heat[j]);
      int pixelnumber;
      if( gReverseDirection ) {
        pixelnumber = (NUM_LEDS_PER_STRIP-1) - j;
      } else {
        pixelnumber = j;
      }
      leds[pixelnumber] = color;
    }

    //Copy first NUM-LEDS-PER_STRIP to the other
    for (int k = 0; k < NUM_STRIPS; k++) {
      for(int l = 0; l < NUM_LEDS_PER_STRIP; l++) {
        leds[k*NUM_LEDS_PER_STRIP + l] = leds[l];
      }
    }

  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);
  delay(10);
  LEDS.show(); // display this frame
  LEDS.delay(1000 / FRAMES_PER_SECOND);
  delay(10);
  
  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);
  delay(10);
  LEDS.show(); // display this frame
  LEDS.delay(1000 / FRAMES_PER_SECOND);
  delay(10);  

  digitalWrite(pinA, LOW);
  digitalWrite(pinB, HIGH);
  digitalWrite(pinC, LOW);
  delay(10);  
  LEDS.show(); // display this frame
  LEDS.delay(1000 / FRAMES_PER_SECOND);
  delay(10);  
}

int counter = 0;
int wait = 10000;

void one_color() 
{
  counter++;
  counter = counter%3;

    for (int k = 0; k < NUM_STRIPS; k++) {
      for(int l = 0; l < NUM_LEDS_PER_STRIP; l++) {
        if (counter == 0)
          leds[k*NUM_LEDS_PER_STRIP + l] = CRGB::Yellow;
        else if (counter == 1)
          leds[k*NUM_LEDS_PER_STRIP + l] = CRGB::Blue;
          else
            leds[k*NUM_LEDS_PER_STRIP + l] = CRGB::Yellow;
      }
    }


  
  //Lower
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);

  delay(500);

  LEDS.show(); // display this frame

  LEDS.delay(wait);


      //Copy first NUM-LEDS-PER_STRIP to the other
    for (int k = 0; k < NUM_STRIPS; k++) {
      for(int l = 0; l < NUM_LEDS_PER_STRIP; l++) {
        if (counter == 0)
          leds[k*NUM_LEDS_PER_STRIP + l] = CRGB::Yellow;
        else if (counter == 1)
          leds[k*NUM_LEDS_PER_STRIP + l] = CRGB::Blue;
          else
            leds[k*NUM_LEDS_PER_STRIP + l] = CRGB::Yellow;
      }
    }


//middle
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, HIGH);

  delay(500);
  
  LEDS.show();
  
  LEDS.delay(wait);


    //Copy first NUM-LEDS-PER_STRIP to the other
    for (int k = 0; k < NUM_STRIPS; k++) {
      for(int l = 0; l < NUM_LEDS_PER_STRIP; l++) {
        if (counter == 0)
          leds[k*NUM_LEDS_PER_STRIP + l] = CRGB::Yellow;
        else if (counter == 1)
          leds[k*NUM_LEDS_PER_STRIP + l] = CRGB::Blue;
          else
            leds[k*NUM_LEDS_PER_STRIP + l] = CRGB::Yellow;
      }
    }

//upper
  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, LOW);

  delay(500);
  
  LEDS.show();

  LEDS.delay(wait);
 
}

void test() {
  counter = counter%3;

    for (int k = 0; k < NUM_STRIPS; k++) {
      for(int l = 0; l < NUM_LEDS_PER_STRIP; l++) {
        if (counter == 0)
          leds[k*NUM_LEDS_PER_STRIP + l] = CRGB::Blue;
        else if (counter == 1)
          leds[k*NUM_LEDS_PER_STRIP + l] = CRGB::Yellow;
          else
            leds[k*NUM_LEDS_PER_STRIP + l] = CRGB::Red;
      }
    }
    
  //Lower
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);

  delay(500);

  LEDS.show(); // display this frame

  LEDS.delay(wait);

  counter++;
}

void loop() {
  //one_color();
  Fire2012();
  //test();
}
