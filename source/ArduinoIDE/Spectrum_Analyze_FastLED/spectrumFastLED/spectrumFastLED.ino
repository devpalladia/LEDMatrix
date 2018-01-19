#define ARM_MATH_CM4
#include <arm_math.h>
#include <FastLED.h>

////////////////////////////////////////////////////////////////////////////////
// CONFIGURABLE VARIABLES -     LED MATRIX
int MODE = 1;                               // MODES: 1=SPECTRUM, 2=STATIC COLOR
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
int ROSA = 0xF5007A;                 // COLOR 1
int WEIS = 0xd4d4d4d;                 // COLOR 2
int BLAU = 0x000066;                 // COLOR 3
int GRUEN = 0x00AF33;
int GELB = 0xFFFF19;
float DENSITY = 0.5;                        // Value to tell how many LED per strip should be activated. Example 0.5 = 50% = Every Second LED

////////////////////////////////////////////////////////////////////////////////
// CONFIGURABLE VARIABLES -     FFT
int SAMPLE_RATE_HZ = 9000;             // Sample rate of the audio in hertz.
// NOT CONFIGURABLE VARIABLES - FFT
const int FFT_SIZE = 256;              // Size of the FFT. Realistically can only be at most 256 without running out of memory for buffers and other state.
const int AUDIO_INPUT_MIC_PIN = 16;    // Input ADC pin for audio data from Microphone
const int ANALOG_READ_RESOLUTION = 10; // Bits of resolution for the ADC.
const int ANALOG_READ_AVERAGING = 16;  // Number of samples to average with each ADC reading.
float samples[FFT_SIZE*2];
float magnitudes[FFT_SIZE];
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// NOT CONFIGURABLE VARIABLES - LED MATRIX
/*
 * A=0; B=0; C = 0 -> Lower LED Panel
 * A=1; B=0; C = 0 -> Middle LED Panel
 * A=0; B=1; C = 0 -> Upper LED Panel
 */
int pinA = 20;
int pinB = 21;
int pinC = 17;
#define NUM_LEDS_PER_STRIP 139
#define NUM_STRIPS 12
#define BRIGHTNESS  50
#define FRAMES_PER_SECOND 60
// Pin layouts on the teensy 3/3.1:
// WS2811_PORTD: 2,14,7,8,6,20,21,5
// WS2811_PORTC: 15,22,23,9,10,13,11,12,28,27,29,30 (these last 4 are pads on the bottom of the teensy)
// WS2811_PORTDC: 2,14,7,8,6,20,21,5,15,22,23,9,10,13,11,12 - 16 way parallel
CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// SAMPLING AND FFT FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

IntervalTimer samplingTimer;
int sampleCounter = 0;

void samplingSetup() {
  // Set up ADC and audio input.
  pinMode(AUDIO_INPUT_MIC_PIN, INPUT);
  analogReadResolution(ANALOG_READ_RESOLUTION);
  analogReadAveraging(ANALOG_READ_AVERAGING);
}

////////////////////////////////////////////////////////////////////////////////
// SAMPLING FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
void samplingCallback() {
  // Read from the ADC and store the sample data
  samples[sampleCounter] = (float32_t)analogRead(AUDIO_INPUT_MIC_PIN);
  // Complex FFT functions require a coefficient for the imaginary part of the input.
  // Since we only have real data, set this coefficient to zero.
  samples[sampleCounter+1] = 0.0;
  // Update sample buffer position and stop after the buffer is filled
  sampleCounter += 2;
  if (sampleCounter >= FFT_SIZE*2) {
    samplingTimer.end();
  }
}

void samplingBegin() {
  // Reset sample buffer position and start callback at necessary rate.
  sampleCounter = 0;
  samplingTimer.begin(samplingCallback, 1000000/SAMPLE_RATE_HZ);
}

boolean samplingIsDone() {
  return sampleCounter >= FFT_SIZE*2;
}

void performFFT() {
  // Run FFT on sample data.
  arm_cfft_radix4_instance_f32 fft_inst;
  arm_cfft_radix4_init_f32(&fft_inst, FFT_SIZE, 0, 1);
  arm_cfft_radix4_f32(&fft_inst, samples);
  // Calculate magnitude of complex numbers output by the FFT.
  arm_cmplx_mag_f32(samples, magnitudes, FFT_SIZE);
}
////////////////////////////////////////////////////////////////////////////////
// !!!!SAMPLING FUNCTIONS!!!!
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// MATRIX FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
enum {LOWER, MIDDLE, UPPER};
typedef struct {
  int panel;
  int leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];
}Panel;

Panel lower, middle, upper;

void showPanel(Panel *panel, int bright = BRIGHTNESS) {
  //Set brightness
  LEDS.setBrightness(bright);
  
  //Select Multiplexer output
  if (panel->panel == LOWER) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, LOW);
  } else if(panel->panel == MIDDLE) {
    digitalWrite(pinA, HIGH);
    digitalWrite(pinB, LOW);
    digitalWrite(pinC, LOW);
  } else if(panel->panel == UPPER) {
    digitalWrite(pinA, LOW);
    digitalWrite(pinB, HIGH);
    digitalWrite(pinC, LOW);
  }
  delay(1); //Delay to give pins time to adjust

  //Copy first NUM-LEDS-PER_STRIP to the other
  for (int k = 0; k < NUM_STRIPS; k++) {
    for(int l = 0; l < NUM_LEDS_PER_STRIP; l++) {
      leds[k*NUM_LEDS_PER_STRIP + l] = panel->leds[k][l];
    }
  }

  //Display
  LEDS.show(); // display this frame
  delay(5);
}

////////////////////////////////////////////////////////////////////////////////
// INIT FUNCTIONS
////////////////////////////////////////////////////////////////////////////////
void initMultiplexer() {
  pinMode(pinA, OUTPUT);
  pinMode(pinB, OUTPUT);
  pinMode(pinC, OUTPUT);
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, LOW);
  digitalWrite(pinC, LOW);
}

void initFastLED() {
  LEDS.addLeds<WS2811_PORTC,NUM_STRIPS>(leds, NUM_LEDS_PER_STRIP);
  LEDS.setBrightness(BRIGHTNESS);
}


void initMatrix() {
  lower.panel = LOWER;
  middle.panel = MIDDLE;
  upper.panel = UPPER;

    //Copy color to all leds in matrix
  for (int k = 0; k < NUM_STRIPS; k++) {
    for(int l = 0; l < NUM_LEDS_PER_STRIP; l++) {
      upper.leds[k][l] = ROSA;
    }
  }

    //Copy color to all leds in matrix
  for (int k = 0; k < NUM_STRIPS; k++) {
    for(int l = 0; l < NUM_LEDS_PER_STRIP; l++) {
      middle.leds[k][l] = WEIS;
    }
  }

    //Copy color to all leds in matrix
  for (int k = 0; k < NUM_STRIPS; k++) {
    for(int l = 0; l < NUM_LEDS_PER_STRIP; l++) {
      lower.leds[k][l] = BLAU;
    }
  }
  //Fade in LEDS
  for (int i = 20; i <= BRIGHTNESS; i++) {
    showPanel(&upper, i);
    showPanel(&middle, i);
    showPanel(&lower, i);
    delay(100);
  }
  //Fade out LEDS

  for (int i = BRIGHTNESS; i >= 20; i--) {
    showPanel(&upper, i);
    showPanel(&middle, i);
    showPanel(&lower, i);
    delay(100);
  }
}
////////////////////////////////////////////////////////////////////////////////
// !!!!INIT FUNCTIONS!!!!
////////////////////////////////////////////////////////////////////////////////

unsigned long time1 = 0;
unsigned long time2 = 0;
int bright = 0;
void setup() {
  // Init - electrical warm-up
  delay(3000);

  // Init of pins for Multiplexer
  initMultiplexer();

  // Init of FastLED Library
  initFastLED();

  // Init Matrix - Fade in LEDS
  initMatrix();

  // Init loop
  bright = 50;
  showPanel(&upper, bright);
  showPanel(&middle, bright);
  showPanel(&lower, bright);
}

int counter = 0;
int timeintervall = 30 * 60 * 1000; //millis
void loop() {
  time1 = millis();
  if ((time1 - time2) >= timeintervall) {
    counter++;
    counter = counter%3;
    if (counter == 0) {
      bright = 50;
    } else if (counter == 1) {
      LEDS.setBrightness(BRIGHTNESS);
      bright = 80;
    } else if (counter == 2) {
      bright = 35;
    }
    time2 = time1;
  }

  showPanel(&upper, bright);
  showPanel(&middle, bright);
  showPanel(&lower, bright);

  delay(100);
}
