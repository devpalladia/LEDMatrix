#define ARM_MATH_CM4
#include <arm_math.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

////////////////////////////////////////////////////////////////////////////////
// NOT CONFIGURABLE VARIABLES - LED MATRIX
const uint8_t NEO_PIXEL_TYP = NEO_GRB + NEO_KHZ800;    
const int     NEO_PIXEL_COUNT = 139;                // Number of neo pixels per strip
const int     STRIP_COUNT = 12;                     // Number of neo pixel strips - SEE led_matrix tab for details
const int     VERTICAL_DIV = 3;                     // Divide stripes in X=VERTICAL_DIV parts. X=1 all stripes will display the same color. TODO MAKE CONFIGURABLE
////////////////////////////////////////////////////////////////////////////////
// CONFIGURABLE VARIABLES -     LED MATRIX
int MODE = 2;                               // MODES: 1=SPECTRUM, 2=STATIC COLOR
//Mode 1
float SPECTRUM_MIN_DB = 40.0;               // Audio intensity (in decibels) that maps to low LED brightness.
float SPECTRUM_MAX_DB = 70.0;               // Audio intensity (in decibels) that maps to high LED brightness.
int   BAR_WIDTH = 5;                        // Width of one bar (column) - default 1 Pixel (LED)
int   HSV_COLOR_MIN = 120;                  // HSV color lower bound in degrees
int   HSV_COLOR_MAX = 0;                    // HSV color upper bound in degrees
float SATURATION = 0.5;                     // HSV color saturation value
//Mode 2
//int HEX_COLOR_1 = 0x9C1F5C;
//int HEX_COLOR_2 = 0x8026746;
//int HEX_COLOR_3 = 0x020224;
int HEX_COLOR_1 = 0xF5007A;                 // COLOR 1
int HEX_COLOR_2 = 0xd4d4d4d;                 // COLOR 2
int HEX_COLOR_3 = 0x000066;                 // COLOR 3
//int HEX_COLOR_1 = 0x006400;                 // COLOR 1
//int HEX_COLOR_2 = 0xFFFF00;                 // COLOR 2
//int HEX_COLOR_3 = 0x000066;                 // COLOR 3
float DENSITY = 0.5;                        // Value to tell how many LED per strip should be activated. Example 0.5 = 50% = Every Second LED


/*
 * A=0; B=0 -> Lower LED Panel
 * A=0; B=1 -> Middle LED Panel
 * A=1; B=0 -> Upper LED Panel
 * 
 */
 
int pinA = 20;
int pinB = 21;

void setup() {

  delay(1000);
  
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);

  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);

  delay(100);

  initialize_matrix();

  digitalWrite(pinA, LOW);
  digitalWrite(pinB, HIGH);

  delay(100);

  initialize_matrix();
  
  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, LOW);

  delay(100);

  initialize_matrix();

  /*
  delay(100);

  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);

  delay(300);

  display_color(HEX_COLOR_3);

  digitalWrite(pinA, LOW);
  digitalWrite(pinB, HIGH);

  delay(300);

  display_color(HEX_COLOR_2);
  
  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, LOW);

  delay(300);

  display_color(HEX_COLOR_1);
  */
}

void loop() {
/*
  delay(100);

  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);

  delay(300);

  display_color(HEX_COLOR_3);

  digitalWrite(pinA, LOW);
  digitalWrite(pinB, HIGH);

  delay(300);

  display_color(HEX_COLOR_2);
  
  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, LOW);

  delay(300);

  display_color(HEX_COLOR_1);

  delay(5000);
*/
/*
  delay(100);
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
  delay(300);
  display_color(HEX_COLOR_3);

  delay(100);
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, HIGH);
  delay(100);
  display_color(HEX_COLOR_3);

  delay(100);
  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, LOW);
  delay(100);
  display_color(HEX_COLOR_3);
*/

  delay(100);
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
  test(0x000066);

  delay(100);
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, HIGH);
  test(0x000066);

  delay(100);
  digitalWrite(pinA, HIGH);
  digitalWrite(pinB, LOW);
  test(0x000066);

}

