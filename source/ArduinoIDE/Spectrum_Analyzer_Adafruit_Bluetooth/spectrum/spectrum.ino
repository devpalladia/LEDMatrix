// Audio Spectrum Display
// Copyright 2013 Tony DiCola (tony@tonydicola.com). Modified by Michael Gremzo 

// This code is part of the guide at http://learn.adafruit.com/fft-fun-with-fourier-transforms/

#define ARM_MATH_CM4
#include <arm_math.h>
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

////////////////////////////////////////////////////////////////////////////////
// DECLARE VARIABLES HERE TO BE SEEN BY ALL TABS! //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// CONFIGURABLE VARIABLES -     LED MATRIX
int MODE = 2;                               // MODES: 1=SPECTRUM, 2=STATIC COLOR
//Mode 1
float SPECTRUM_MIN_DB = 40.0;               // Audio intensity (in decibels) that maps to low LED brightness.
float SPECTRUM_MAX_DB = 70.0;               // Audio intensity (in decibels) that maps to high LED brightness.
int   BAR_WIDTH = 5;                        // Width of one bar (column) - default 1 Pixel (LED)
int   HSV_COLOR_MIN = 120;                  // HSV color lower bound in degrees
int   HSV_COLOR_MAX = 0;                    // HSV color upper bound in degrees
float SATURATION = 1.0;                     // HSV color saturation value
//Mode 2
int HEX_COLOR_1 = 0x9C1F5C;
int HEX_COLOR_2 = 0x8026746;
int HEX_COLOR_3 = 0x020224;
//int HEX_COLOR_1 = 0xF5007A;                 // COLOR 1
//int HEX_COLOR_2 = 0xFFFFFF;                 // COLOR 2
//int HEX_COLOR_3 = 0x000066;                 // COLOR 3
//int HEX_COLOR_1 = 0x006400;                 // COLOR 1
//int HEX_COLOR_2 = 0xFFFF00;                 // COLOR 2
//int HEX_COLOR_3 = 0x000066;                 // COLOR 3
float DENSITY = 0.5;                        // Value to tell how many LED per strip should be activated. Example 0.5 = 50% = Every Second LED

// NOT CONFIGURABLE VARIABLES - LED MATRIX
const uint8_t NEO_PIXEL_TYP = NEO_GRB + NEO_KHZ800;    
const int     NEO_PIXEL_COUNT = 139;                // Number of neo pixels per strip
const int     STRIP_COUNT = 12;                     // Number of neo pixel strips - SEE led_matrix tab for details
const int     VERTICAL_DIV = 3;                     // Divide stripes in X=VERTICAL_DIV parts. X=1 all stripes will display the same color. TODO MAKE CONFIGURABLE
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// NOT CONFIGURABLE VARIABLES - COMMANDS
const int MAX_CHARS = 65;                   // Max size of the input command buffer
char commandBuffer[MAX_CHARS];              // Input Buffer
const int POWER_LED_PIN = 13;               // TEMPORARILY !!! JUST FOR DEBUGGING
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// CONFIGURABLE VARIABLES -     FFT
int SAMPLE_RATE_HZ = 9000;             // Sample rate of the audio in hertz.

// NOT CONFIGURABLE VARIABLES - FFT
const int FFT_SIZE = 256;              // Size of the FFT. Realistically can only be at most 256 without running out of memory for buffers and other state.
const int AUDIO_INPUT_MIC_PIN = 21;    // Input ADC pin for audio data from Microphone
const int AUDIO_INPUT_L_PIN = 20;      // Input ADC pin for audio data from Audio Line - Left Channel
const int AUDIO_INPUT_R_PIN = 19;      // Input ADC pin for audio data from Audio Line - Right Channel
const int ANALOG_READ_RESOLUTION = 10; // Bits of resolution for the ADC.
const int ANALOG_READ_AVERAGING = 16;  // Number of samples to average with each ADC reading.
float samples[FFT_SIZE*2];
float magnitudes[FFT_SIZE];
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// NOT CONFIGURABLE VARIABLES - BLUETOOTH
const int UART1_TX = 1;               // TX Pin for UART 1 - Connected to Bluetooth Modul RX Pin
const int UART1_RX = 0;               // RX Pin for UART 1 - Connected to Bluetooth Modul TX Pin
const int BAUDRATE_BT = 115200;       // Baudrate for communication with Bluetooth Modul over UART1
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// NOT CONFIGURABLE VARIABLES - SERIAL
const int BAUDRATE_SL = 38400;        // Baudrate for communication with Host computer over Serial
////////////////////////////////////////////////////////////////////////////////

//DEBUG FLAG
bool DEBUG = false;

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
  
  // Setup the Communication Channels
  serialSetup();
  bluetoothSetup();

  Serial.println("Hello from Teensy!");
  
  // Initialize Analog Sampling
  samplingSetup();

  // Initialize Spectrum Variables
  spectrumSetup();

  // Initialze LED Matrix
  ledmatrixSetup();
  
  // Clear the input command buffer
  memset(commandBuffer, 0, sizeof(commandBuffer));

}

////////////////////////////////////////////////////////////////////////////////
// LOOP STRUCTURE
////////////////////////////////////////////////////////////////////////////////

int lastMODE = 0;
unsigned long time1 = 0;
unsigned long time2 = 0;
int counter = 0;

void loop() {
  //Check which Mode is selected
  if (MODE == 1) {
    //Start Sampling if entered MODE 1 first time!
    if (lastMODE != 1) {
      samplingBegin();
    }
    //Check if sampling is done
    if (samplingIsDone()) {
      performFFT();

      // digitalWrite(POWER_LED_PIN, !digitalRead(POWER_LED_PIN));
      // time2 = micros();
      // Serial.println(time2-time1);
      // time1 = time2;

      //Run the Display loop
      spectrum_displayLoop();

      //Display static color on other panels every Xth time
      if (counter == 10) {
        display_color_1();
        display_color_2();
        counter = 0;
      }
      counter++;
      
      //Restart Sampling 
      samplingBegin();

      //digitalWrite(POWER_LED_PIN, !digitalRead(POWER_LED_PIN));
      //Serial.println("Hello from Teensy!");
    }
    lastMODE = 1;
  } 
  else if (MODE == 2) {
    if (lastMODE != 2) {
      //Setup 
      single_color_setup();
      counter = 0;
    }
    if (counter == 10) {
      single_color_setup();
      counter = 0;
    }
    counter++;
    lastMODE = 2;
  }
  else {
    lastMODE = MODE;
  }
  
  //Check Communication Channels
  serialLoop();
  bluetoothLoop();

}
