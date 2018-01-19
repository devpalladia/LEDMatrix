#include "Arduino.h"
#include <FastLED.h>
#include "bitmaps/birthday.h"
#include "bitmaps/flags.h"

int PALLADIA_ROSA = 0x9C1F5C;
int PALLADIA_WEIS = 0xd4d4d4;
int PALLADIA_BLAU = 0x020224;

#define NUM_LEDS_PER_STRIP 139
#define NUM_STRIPS 12
#define BRIGHTNESS 70

int pinA = 20;
int pinB = 21;
int pinC = 17;

int button_yellow = 3;
int button_green = 4;
int button_blue = 5;

// ################### BITMAPS ###################
//Palladia, Schland, Russland, Spanien, Mexico, England, Brasilien
#define BITMAPS 3
int bitmap_counter = 0;
const int brightness_array[BITMAPS] = {50, 50, 50};
typedef const int (*bitmap_ptr_t)[139];
bitmap_ptr_t bitmap_ptr[BITMAPS] = {palladia, palladia21, palladia19};
//#########################################################

int delay_ = 5; //Milli seconds

int led = 13;

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

void setColor(int color) {
	//Copy first NUM-LEDS-PER_STRIP to the other
	for (int k = 0; k < NUM_STRIPS; k++) {
		for(int l = 0; l < NUM_LEDS_PER_STRIP; l++) {
		  leds[k*NUM_LEDS_PER_STRIP + l] = color;
		}
	}
}

void setBitMap(const int bitmap[36][139], int panel) { //panel: 0->high, 1->middle, 2->low
	LEDS.clearData();
	if (panel == 2) //hardware exception handling -> inverse wiring
	{
		for (int k = 0; k < NUM_STRIPS; k++) {
			for(int l = 0; l < NUM_LEDS_PER_STRIP; l++) {
				leds[k*NUM_LEDS_PER_STRIP + l] = bitmap[(panel+1)*NUM_STRIPS - k - 1][l];
			}
		}
		return;
	}
	for (int k = 0; k < NUM_STRIPS; k++) {
		for(int l = 0; l < NUM_LEDS_PER_STRIP; l++) {
			leds[k*NUM_LEDS_PER_STRIP + l] = bitmap[panel*NUM_STRIPS + k][l];
		}
	}
}

void setBitMap_modded(const int bitmap[36][139], int panel) { //panel: 0->high, 1->middle, 2->low
	LEDS.clearData();
	if (panel == 2) //hardware exception handling -> inverse wiring
	{
		for (int k = 0; k < NUM_STRIPS; k++) {
			for(int l = 0; l < NUM_LEDS_PER_STRIP; l++) {
				leds[k*NUM_LEDS_PER_STRIP + l] = bitmap[(panel+1)*NUM_STRIPS - k - 1][l];
			}
		}
		return;
	}
	for (int k = 0; k < NUM_STRIPS; k++) {
		for(int l = 0; l < NUM_LEDS_PER_STRIP; l++) {
			leds[k*NUM_LEDS_PER_STRIP + l] = bitmap[panel*NUM_STRIPS + k][l];
		}
	}
}

void display () {
	//Enter critical Section
	cli();

	//LOW
	digitalWrite(pinA, LOW);
	digitalWrite(pinB, LOW);
	digitalWrite(pinC, LOW);
	setBitMap(bitmap_ptr[bitmap_counter], 2);
	LEDS.show();
	delay(delay_);

	//MIDDLE
	digitalWrite(pinA, HIGH);
	digitalWrite(pinB, LOW);
	digitalWrite(pinC, LOW);
	setBitMap(bitmap_ptr[bitmap_counter], 1);
	LEDS.show();
	delay(delay_);

	//UP
	digitalWrite(pinA, LOW);
	digitalWrite(pinB, HIGH);
	digitalWrite(pinC, LOW);
	setBitMap(bitmap_ptr[bitmap_counter], 0);
	LEDS.show();
	delay(delay_);

	//Leave critical Section
	sei();
}

//The setup function is called once at startup of the sketch
void setup()
{
	// Init - electrical warm-up
	delay(3000);

	pinMode(pinA, OUTPUT);
	pinMode(pinB, OUTPUT);
	pinMode(pinC, OUTPUT);

	pinMode(button_yellow, INPUT);
	pinMode(button_green, INPUT);
	pinMode(button_blue, INPUT);

	attachInterrupt(button_yellow, isrServiceYellow, FALLING);
	//attachInterrupt(button_green, isrServiceGreen, FALLING);
	//attachInterrupt(button_blue, isrServiceBlue, FALLING);

	LEDS.addLeds<WS2811_PORTC,NUM_STRIPS>(leds, NUM_LEDS_PER_STRIP);

	//Set Brightness
	LEDS.setBrightness(brightness_array[bitmap_counter]);

	display();

	delay(2000);
}

// The loop function is called in an endless loop
void loop()
{
	//Display - rewrite LEDs
	display();

	//Wait - control refresh rate
	delay(500);
}

void isrServiceYellow()
{
	bitmap_counter++; //Next bitmap
	bitmap_counter = bitmap_counter%BITMAPS;
}

void isrServiceGreen()
{
	digitalWrite(led, !digitalRead(led));
}

void isrServiceBlue()
{
	digitalWrite(led, !digitalRead(led));
}
