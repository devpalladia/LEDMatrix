#include <FastLED.h>

int PALLADIA_ROSA = 0x9C1F5C;
int PALLADIA_WEIS = 0xd4d4d4;
int PALLADIA_BLAU = 0x020224;

#define NUM_LEDS_PER_STRIP 139
#define NUM_STRIPS 12
#define BRIGHTNESS 70

int pinA = 20;
int pinB = 21;
int pinC = 17;

int delay_ = 5; //Milli seconds

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

void setColor(int color) {
  //Copy first NUM-LEDS-PER_STRIP to the other
  for (int k = 0; k < NUM_STRIPS; k++) {
    for(int l = 0; l < NUM_LEDS_PER_STRIP; l++) {
      leds[k*NUM_LEDS_PER_STRIP + l] = color;
    }
  }
}

void display () {
  //Display
    //LOW
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);
  setColor(PALLADIA_BLAU);
  LEDS.show();
  delay(delay_);
    //MIDDLE
  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);
  setColor(PALLADIA_WEIS);
  LEDS.show();
  delay(delay_);
    //UP
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, HIGH);
  digitalWrite(pinC, LOW);
  setColor(PALLADIA_ROSA);
  LEDS.show();
  delay(delay_);
}

void setup()
{
  // Init - electrical warm-up
  delay(3000);

  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);

  LEDS.addLeds<WS2811_PORTC,NUM_STRIPS>(leds, NUM_LEDS_PER_STRIP);

  //Set Brightness
  LEDS.setBrightness(70);

  display();

  delay(2000);
}

void loop()
{
  //Display - rewrite LEDs
  display();

  //Wait
  delay(500);
}
