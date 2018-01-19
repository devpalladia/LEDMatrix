////////////////////////////////////////////////////////////////////////////////
// LED AND SPECTRUM DISPLAY FUNCTIONS
// Hardware setup of LED stripes
// --------------------------2,4m * 60 rgb/m = 144--------------------------
// |---------------------------- LED Stripe 12 ----------------------------|
// |---------------------------- LED Stripe 11 ----------------------------|
// |---------------------------- LED Stripe 10 ----------------------------|
// |---------------------------- LED Stripe 9  ----------------------------|
// |---------------------------- LED Stripe 8  ----------------------------|
// |---------------------------- LED Stripe 7  ----------------------------|
// |---------------------------- LED Stripe 6  ----------------------------|
// |---------------------------- LED Stripe 5  ----------------------------|
// |---------------------------- LED Stripe 4  ----------------------------|
// |---------------------------- LED Stripe 3  ----------------------------|
// |---------------------------- LED Stripe 2  ----------------------------|
// |---------------------------- LED Stripe 1  ----------------------------|
// -------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
const int NEO_PIXEL_PIN_1 = 15;
const int NEO_PIXEL_PIN_2 = 22;
const int NEO_PIXEL_PIN_3 = 23;
const int NEO_PIXEL_PIN_4 = 9;
const int NEO_PIXEL_PIN_5 = 10;
const int NEO_PIXEL_PIN_6 = 13;
const int NEO_PIXEL_PIN_7 = 11;
const int NEO_PIXEL_PIN_8 = 12;
const int NEO_PIXEL_PIN_9 = 28;
const int NEO_PIXEL_PIN_10 = 27;
const int NEO_PIXEL_PIN_11 = 29;
const int NEO_PIXEL_PIN_12 = 30;
const int NEO_PIXEL_PIN_13_24 = 7;
const int NEO_PIXEL_PIN_25_36 = 8; 
////////////////////////////////////////////////////////////////////////////////
Adafruit_NeoPixel pixels_1 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_1, NEO_PIXEL_TYP);
Adafruit_NeoPixel pixels_2 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_2, NEO_PIXEL_TYP);
Adafruit_NeoPixel pixels_3 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_3, NEO_PIXEL_TYP);
Adafruit_NeoPixel pixels_4 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_4, NEO_PIXEL_TYP);
Adafruit_NeoPixel pixels_5 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_5, NEO_PIXEL_TYP);
Adafruit_NeoPixel pixels_6 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_6, NEO_PIXEL_TYP);
Adafruit_NeoPixel pixels_7 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_7, NEO_PIXEL_TYP);
Adafruit_NeoPixel pixels_8 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_8, NEO_PIXEL_TYP);
Adafruit_NeoPixel pixels_9 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_9, NEO_PIXEL_TYP);
Adafruit_NeoPixel pixels_10 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_10, NEO_PIXEL_TYP);
Adafruit_NeoPixel pixels_11 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_11, NEO_PIXEL_TYP);
Adafruit_NeoPixel pixels_12 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_12, NEO_PIXEL_TYP);

Adafruit_NeoPixel pixels_13_24 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_13_24, NEO_PIXEL_TYP);
Adafruit_NeoPixel pixels_25_36 = Adafruit_NeoPixel(NEO_PIXEL_COUNT, NEO_PIXEL_PIN_25_36, NEO_PIXEL_TYP);

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Variables for spectrum display
float *frequencyWindow = NULL;
float hues[STRIP_COUNT];
int windowCount = 0;
uint32_t matrix_colors[STRIP_COUNT][NEO_PIXEL_COUNT];
uint32_t bar_colors[STRIP_COUNT];
float columns = 0;
float windowSize = 0;
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// SPECTRUM DISPLAY FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void ledmatrixSetup() {
  // Initialize neo pixel library and turn off the LEDs
  initialize_matrix();
}

////////////////////////////////////////////////////////////////////////////////
// SPECTRUM DISPLAY FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void spectrumSetup() {
  ////////////////////////////////////////////////////////////////////////////////
  // SPECTRUM SETUP: Initialize all Variables for spectrum display and CHECK IF ALL VARIABLES ARE IN LIMIT!!!
  ////////////////////////////////////////////////////////////////////////////////

  // CHECK IF ALL CONFIGURABLE VARIABLES ARE IN LIMIT!
  if ((SPECTRUM_MIN_DB < 0) || (SPECTRUM_MIN_DB >= SPECTRUM_MAX_DB) || (SPECTRUM_MIN_DB >= 90))
    SPECTRUM_MIN_DB = 30;

  if ((SPECTRUM_MAX_DB < 0) || (SPECTRUM_MIN_DB >= SPECTRUM_MAX_DB) || (SPECTRUM_MAX_DB >= 90))
    SPECTRUM_MAX_DB = 60;

  if ((BAR_WIDTH <= 0 ) || (BAR_WIDTH > NEO_PIXEL_COUNT/2))
    BAR_WIDTH = 5;

  if ((HSV_COLOR_MIN < 0) || (HSV_COLOR_MIN > 360))
    HSV_COLOR_MIN = 120;

  if ((HSV_COLOR_MAX < 0) || (HSV_COLOR_MAX > 360))
    HSV_COLOR_MAX = 0;

  if ((SATURATION < 0) || (SATURATION > 1.0))
    SATURATION = 1.0;

  if ((HEX_COLOR_1 < 0) || (HEX_COLOR_1 > 0xFFFFFF))
    HEX_COLOR_1 = 0xFF008C;

  if ((HEX_COLOR_2 < 0) || (HEX_COLOR_2 > 0xFFFFFF))
    HEX_COLOR_2 = 0xFFFFFF;

  if ((HEX_COLOR_3 < 0) || (HEX_COLOR_3 > 0xFFFFFF))
    HEX_COLOR_3 = 0x000DFF;

  if ((DENSITY < 0) || (DENSITY > 1.0))
    DENSITY = 0.5;

  if ((SAMPLE_RATE_HZ < 4500) || (SAMPLE_RATE_HZ > 20000))
    SAMPLE_RATE_HZ = 9000;
    
  // Set the frequency window values by evenly dividing the possible frequency
  // spectrum across the number of neo pixels
  columns = float(NEO_PIXEL_COUNT) / float(BAR_WIDTH); 
  windowSize = (SAMPLE_RATE_HZ / 2.0) / columns;
  windowCount = int(ceil(columns));
  
  // Resize fequencyWindow Array
  free(frequencyWindow);
  frequencyWindow = (float*)calloc(windowCount+1, sizeof(float));
  
  for (int i = 0; i < windowCount+1; ++i) {
    frequencyWindow[i] = i*windowSize;
  }
  // Evenly spread hues across all stripes
  int hsv_diff = HSV_COLOR_MAX - HSV_COLOR_MIN;
  if (hsv_diff < 0 && hsv_diff >= -360) {
    for (int j = 0; j < STRIP_COUNT; ++j) {
      hues[j] = HSV_COLOR_MIN + hsv_diff*(float(j)/float(STRIP_COUNT-1));
    }
  } else if (hsv_diff > 0 && hsv_diff <= 360) {
    for (int j = 0; j < STRIP_COUNT; ++j) {
      hues[j] = HSV_COLOR_MIN + hsv_diff*(float(j)/float(STRIP_COUNT-1));
    }
  } else {
    // Go to default values! MIN = 0 && MAX = 360
    HSV_COLOR_MIN = 0;
    HSV_COLOR_MAX = 360;
    for (int j = 0; j < STRIP_COUNT; ++j) {
      hues[j] = 360.0*(float(j)/float(STRIP_COUNT-1));
    }
  }
}

void spectrumSetupDebug() {
  Serial.print("Columns: "); Serial.println(columns);
  Serial.print("WindowSize: "); Serial.println(windowSize);
  Serial.print("WindowCount: "); Serial.println(windowCount);
  Serial.print("Bar Width: "); Serial.println(BAR_WIDTH);
  Serial.print("FrequencyWindow: ");
  for (int i = 0; i < windowCount+1; ++i) {
    Serial.print(frequencyWindow[i]); Serial.print(" ");
  }
  Serial.println(" ");
  Serial.print("Hues: ");
  for (int j = 0; j < STRIP_COUNT; ++j) {
      Serial.print(hues[j]); ; Serial.print(" ");
  }
  Serial.println(" ");
}

void matrixColorDebug() {
  for (int i = 0; i < STRIP_COUNT; i++) {
    for (int j= 0; j < 60; j++) {
      uint32_t color_ = matrix_colors[i][j];
      Serial.print(color_); Serial.print("\t");
    }
    Serial.println(" ");
  }
}

////////////////////////////////////////////////////////////////////////////////
// SPECTRUM DISPLAY FUNCTIONS
///////////////////////////////////////////////////////////////////////////////

void spectrum_displayLoop() {
  // Update each LED based on the intensity of the audio in the associated frequency window
  // Initialize Color Matrix with grad of STRIPE_COUNT X NEO_PIXEL_COUNT
  float intensity, otherMean;
  int column_counter = 0;
  // for(windowCount)
  //  for(BAR_WIDTH)
  //    for(STRIP_COUNT)
  for (int i = 0; i < windowCount; ++i) {
    windowMean(magnitudes, 
               frequencyToBin(frequencyWindow[i], SAMPLE_RATE_HZ, FFT_SIZE),
               frequencyToBin(frequencyWindow[i+1], SAMPLE_RATE_HZ, FFT_SIZE),
               &intensity,
               &otherMean,
               FFT_SIZE);
    // Convert intensity to decibels.
    intensity = 20.0*log10(intensity);
    // Scale the intensity and clamp between 0 and 1.0.
    intensity -= SPECTRUM_MIN_DB;
    intensity = intensity < 0.0 ? 0.0 : intensity;
    intensity /= (SPECTRUM_MAX_DB-SPECTRUM_MIN_DB);
    intensity = intensity > 1.0 ? 1.0 : intensity;

    // Calculate the height of the Bar
    int row_count = int(intensity*STRIP_COUNT);

    //Serial.print("Column: "); Serial.print(i); Serial.print(" Height: "); Serial.println(row_count);

    // Calculate Colors per Row
    // TODO: Make this independant of intensity and move to spectrum Setup Function!
    for ( int u = 0; u < STRIP_COUNT; u++) {
      bar_colors[u] = pixelHSVtoRGBColor(hues[u], SATURATION, intensity);
      if (DEBUG) {Serial.print(" Color: "); Serial.print(u+1); Serial.print(" Value: "); Serial.print(bar_colors[u]);}
    }
    if (DEBUG) {Serial.println(" ");}
    
    // Set colors in the Matrix for specific Column
    for (int column = 0; column < BAR_WIDTH; column++) {
      // Make sure to not exceed max. number of horizontal pixels
      if (column_counter >= NEO_PIXEL_COUNT) break;
      for (int row = 0; row < STRIP_COUNT; row++) {
        if (row < row_count) {
          // Set Color
          matrix_colors[row][column_counter] = bar_colors[row];
        } else {
          // Set Blackground Color
          // TODO: Define Background Color
          matrix_colors[row][column_counter] = HEX_COLOR_3;
        }
      }
      column_counter++;
    }
  }

  // ONLY TEMPORARILY - SHIFT MATRIX TO THE RIGHT AND MIRROR
  //SHIFT
  for (int strip = 0; strip < STRIP_COUNT; strip++) {
    //SHIFT
    for (int steps = 0; steps < int(NEO_PIXEL_COUNT/2); steps++) {
      for (int k = NEO_PIXEL_COUNT-1; k > 0; k--) {   
        matrix_colors[strip][k] = matrix_colors[strip][k-1];
      }
    }
    //MIRROR
    for (int pos = NEO_PIXEL_COUNT-1; pos > int(NEO_PIXEL_COUNT/2); pos--) {
      matrix_colors[strip][NEO_PIXEL_COUNT - pos - 1] = matrix_colors[strip][pos];
    }
  }
  show_matrix();
}

////////////////////////////////////////////////////////////////////////////////
// SINGLE COLOR DISPLAY FUNCTIONS
///////////////////////////////////////////////////////////////////////////////

void single_color_setup() {
  display_color_1();
  display_color_2();
  display_color_3();
}

void display_color_1() {
  int red = 0;
  int blue = 255;
  int green = 0;

  uint32_t color_ = Adafruit_NeoPixel::Color(red, green, blue);
  color_ = HEX_COLOR_1;

  int filling = int(1/DENSITY);
  
  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    if ( i%filling == 0) {
      pixels_25_36.setPixelColor(i,color_);
    }
  }

  pixels_25_36.show();
}

void display_color_2() {
  int red = 0;
  int blue = 255;
  int green = 0;

  uint32_t color_ = Adafruit_NeoPixel::Color(red, green, blue);
  color_ = HEX_COLOR_2;

  int filling = int(1/DENSITY);
  
  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    if ( i%filling == 0) {
      pixels_13_24.setPixelColor(i,color_);
    }
  }
  pixels_13_24.show();
}

void display_color_3() {
  int red = 0;
  int blue = 255;
  int green = 0;

  uint32_t color_ = Adafruit_NeoPixel::Color(red, green, blue);
  color_ = HEX_COLOR_3;

  int filling = int(1/DENSITY);
  
  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    if ( i%filling == 0) {
      pixels_1.setPixelColor(i,color_);
      pixels_2.setPixelColor(i,color_);
      pixels_3.setPixelColor(i,color_);
      pixels_4.setPixelColor(i,color_);
      pixels_5.setPixelColor(i,color_);
      pixels_6.setPixelColor(i,color_);
      pixels_7.setPixelColor(i,color_);
      pixels_8.setPixelColor(i,color_);
      pixels_9.setPixelColor(i,color_);
      pixels_10.setPixelColor(i,color_);
      pixels_11.setPixelColor(i,color_);
      pixels_12.setPixelColor(i,color_);
    }
  }

  pixels_1.show();
  pixels_2.show();
  pixels_3.show();
  pixels_4.show();
  pixels_5.show();
  pixels_6.show();
  pixels_7.show();
  pixels_8.show();
  pixels_9.show();
  pixels_10.show();
  pixels_11.show();
  pixels_12.show();
  
}

////////////////////////////////////////////////////////////////////////////////
// HARDWARE
///////////////////////////////////////////////////////////////////////////////

void initialize_matrix() {
  pixels_1.begin();
  pixels_1.show(); 
  
  pixels_2.begin();
  pixels_2.show(); 
  
  pixels_3.begin();
  pixels_3.show(); 
  
  pixels_4.begin();
  pixels_4.show(); 
  
  pixels_5.begin();
  pixels_5.show();
   
  pixels_6.begin();
  pixels_6.show();
   
  pixels_7.begin();
  pixels_7.show();
   
  pixels_8.begin();
  pixels_8.show();
   
  pixels_9.begin();
  pixels_9.show();
   
  pixels_10.begin();
  pixels_10.show();
   
  pixels_11.begin();
  pixels_11.show();
   
  pixels_12.begin();
  pixels_12.show();

  pixels_13_24.begin();
  pixels_25_36.begin();

  pixels_13_24.show();
  pixels_25_36.show();

  int red = 0;
  int blue = 255;
  int green = 0;

  uint32_t color_ = Adafruit_NeoPixel::Color(red, green, blue);
  color_ = HEX_COLOR_3;

  int filling = int(1/DENSITY);

  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    if ( i%filling == 0) {
      pixels_1.setPixelColor(i,color_);
      pixels_1.show();
      pixels_2.setPixelColor(i,color_);
      pixels_2.show();
      pixels_3.setPixelColor(i,color_);
      pixels_3.show();
      pixels_4.setPixelColor(i,color_);
      pixels_4.show();
      pixels_5.setPixelColor(i,color_);
      pixels_5.show();
      pixels_6.setPixelColor(i,color_);
      pixels_6.show();
      pixels_7.setPixelColor(i,color_);
      pixels_7.show();
      pixels_8.setPixelColor(i,color_);
      pixels_8.show();
      pixels_9.setPixelColor(i,color_);
      pixels_9.show();
      pixels_10.setPixelColor(i,color_);
      pixels_10.show();
      pixels_11.setPixelColor(i,color_);
      pixels_11.show();
      pixels_12.setPixelColor(i,color_);
      pixels_12.show();

      pixels_13_24.setPixelColor(i,HEX_COLOR_2);
      pixels_13_24.show();

      pixels_25_36.setPixelColor(i,HEX_COLOR_1);
      pixels_25_36.show();
      delay(2);
    }
  }
  
  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    if ( i%filling == 0) {
      pixels_1.setPixelColor(i,0);
      pixels_1.show();
      pixels_2.setPixelColor(i,0);
      pixels_2.show();
      pixels_3.setPixelColor(i,0);
      pixels_3.show();
      pixels_4.setPixelColor(i,0);
      pixels_4.show();
      pixels_5.setPixelColor(i,0);
      pixels_5.show();
      pixels_6.setPixelColor(i,0);
      pixels_6.show();
      pixels_7.setPixelColor(i,0);
      pixels_7.show();
      pixels_8.setPixelColor(i,0);
      pixels_8.show();
      pixels_9.setPixelColor(i,0);
      pixels_9.show();
      pixels_10.setPixelColor(i,0);
      pixels_10.show();
      pixels_11.setPixelColor(i,0);
      pixels_11.show();
      pixels_12.setPixelColor(i,0);
      pixels_12.show();
  
      pixels_13_24.setPixelColor(i,0);
      pixels_13_24.show();
  
      pixels_25_36.setPixelColor(i,0);
      pixels_25_36.show();
      
      delay(2);
    }
  }
  
  color_ = Adafruit_NeoPixel::Color(255, 255, 255);
  
  for (int runner = 0; runner < 50; runner++) {
  if( runner%2 == 0) {
      for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
        if ( i%filling == 0) {
          pixels_1.setPixelColor(i,0);
          pixels_2.setPixelColor(i,0);
          pixels_3.setPixelColor(i,0);
          pixels_4.setPixelColor(i,0);
          pixels_5.setPixelColor(i,0);
          pixels_6.setPixelColor(i,0);
          pixels_7.setPixelColor(i,0);
          pixels_8.setPixelColor(i,0);
          pixels_9.setPixelColor(i,0);
          pixels_10.setPixelColor(i,0);
          pixels_11.setPixelColor(i,0);
          pixels_12.setPixelColor(i,0);
          
          pixels_13_24.setPixelColor(i,0);
          pixels_25_36.setPixelColor(i,0);
        }
        else {
          pixels_1.setPixelColor(i,color_);
          pixels_2.setPixelColor(i,color_);
          pixels_3.setPixelColor(i,color_);
          pixels_4.setPixelColor(i,color_);
          pixels_5.setPixelColor(i,color_);
          pixels_6.setPixelColor(i,color_);
          pixels_7.setPixelColor(i,color_);
          pixels_8.setPixelColor(i,color_);
          pixels_9.setPixelColor(i,color_);
          pixels_10.setPixelColor(i,color_);
          pixels_11.setPixelColor(i,color_);
          pixels_12.setPixelColor(i,color_);
          
          pixels_13_24.setPixelColor(i,color_);
          pixels_25_36.setPixelColor(i,color_);
        }
      }
  }
  else {
    for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
          pixels_1.setPixelColor(i,0);
          pixels_2.setPixelColor(i,0);
          pixels_3.setPixelColor(i,0);
          pixels_4.setPixelColor(i,0);
          pixels_5.setPixelColor(i,0);
          pixels_6.setPixelColor(i,0);
          pixels_7.setPixelColor(i,0);
          pixels_8.setPixelColor(i,0);
          pixels_9.setPixelColor(i,0);
          pixels_10.setPixelColor(i,0);
          pixels_11.setPixelColor(i,0);
          pixels_12.setPixelColor(i,0);
          
          pixels_13_24.setPixelColor(i,0);
          pixels_25_36.setPixelColor(i,0);
        }
    }
      pixels_1.show();
      pixels_2.show();
      pixels_3.show();
      pixels_4.show();
      pixels_5.show();
      pixels_6.show();
      pixels_7.show();
      pixels_8.show();
      pixels_9.show();
      pixels_10.show();
      pixels_11.show();
      pixels_12.show();

      pixels_13_24.show();
      pixels_25_36.show();
  }
  delay(5);
}

void show_matrix () {
  //matrix ist called: colors

  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    pixels_1.setPixelColor(i,matrix_colors[0][i]);
  }

  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    pixels_2.setPixelColor(i,matrix_colors[1][i]);
  }

  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    pixels_3.setPixelColor(i,matrix_colors[2][i]);
  }

  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    pixels_4.setPixelColor(i,matrix_colors[3][i]);
  }

  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    pixels_5.setPixelColor(i,matrix_colors[4][i]);
  }

  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    pixels_6.setPixelColor(i,matrix_colors[5][i]);
  }

  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    pixels_7.setPixelColor(i,matrix_colors[6][i]);
  }

  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    pixels_8.setPixelColor(i,matrix_colors[7][i]);
  }

  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    pixels_9.setPixelColor(i,matrix_colors[8][i]);
  }

  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    pixels_10.setPixelColor(i,matrix_colors[9][i]);
  }

  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    pixels_11.setPixelColor(i,matrix_colors[10][i]);
  }

  for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
    pixels_12.setPixelColor(i,matrix_colors[11][i]);
  }

  pixels_1.show();
  pixels_2.show();
  pixels_3.show();
  pixels_4.show();
  pixels_5.show();
  pixels_6.show();
  pixels_7.show();
  pixels_8.show();
  pixels_9.show();
  pixels_10.show();
  pixels_11.show();
  pixels_12.show();

}

