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

////////////////////////////////////////////////////////////////////////////////

uint32_t matrix_colors[STRIP_COUNT][NEO_PIXEL_COUNT];

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

void display_color(int color_in) {
  uint32_t color_ = color_in;

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

void test(int color_in) {
  uint32_t color_ = color_in;
  for (int j = 0; j < NEO_PIXEL_COUNT; j++) {
    for (int i = 0; i < NEO_PIXEL_COUNT; i++) {
      if (j > i) {
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
      }
      else
      {
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
      }
    
      pixels_1.show();
      /*
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
      */
    }
  }
}

