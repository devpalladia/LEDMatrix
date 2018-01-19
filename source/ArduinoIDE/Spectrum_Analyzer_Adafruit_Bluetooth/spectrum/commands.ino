////////////////////////////////////////////////////////////////////////////////
// COMMAND PARSING FUNCTIONS
// These functions allow parsing simple commands input on the serial port or bluetooth port.
// Commands allow reading and writing variables that control the device.
//
// All commands must end with a semicolon character.
// 
// Example commands are:
// GET SAMPLE_RATE_HZ;
// - Get the sample rate of the device.
// SET SAMPLE_RATE_HZ 400;
// - Set the sample rate of the device to 400 hertz.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Macro used in parseCommand function to simplify parsing get and set commands for a variable
////////////////////////////////////////////////////////////////////////////////
#define GET_AND_SET(variableName) \
  else if (strcmp(command, "GET " #variableName) == 0) { \
    Serial.println(variableName); \
    bluetooth.println(variableName); \
  } \
  else if (strstr(command, "SET " #variableName " ") != NULL) { \
    variableName = (typeof(variableName)) atof(command+(sizeof("SET " #variableName " ")-1)); \
  }

////////////////////////////////////////////////////////////////////////////////
// Function to parse received command string
////////////////////////////////////////////////////////////////////////////////
void parseCommand(char* command) {
  if (strcmp(command, "GET FFT_SIZE") == 0) {
    Serial.println(FFT_SIZE);
    bluetooth.println(FFT_SIZE);
  }
  else if (strcmp(command, "DEBUG SPECTRUM_SETUP") == 0) {
    spectrumSetupDebug();
  }
  else if (strcmp(command, "GET MATRIX_COLOR") == 0) {
    matrixColorDebug();
  }
  GET_AND_SET(SAMPLE_RATE_HZ)
  GET_AND_SET(SPECTRUM_MIN_DB)
  GET_AND_SET(SPECTRUM_MAX_DB)
  GET_AND_SET(BAR_WIDTH)
  GET_AND_SET(HSV_COLOR_MIN)
  GET_AND_SET(HSV_COLOR_MAX)
  GET_AND_SET(SATURATION)
  GET_AND_SET(HEX_COLOR_1)
  GET_AND_SET(HEX_COLOR_2)
  GET_AND_SET(HEX_COLOR_3)
  GET_AND_SET(DENSITY)
  GET_AND_SET(MODE)
  GET_AND_SET(DEBUG)

  // Update spectrum display values if anything has been set
  if (strstr(command, "SET ") != NULL) {
    spectrumSetup();

    //TODO: Could create problems, if MODE hasn't been changed
    if (MODE == 2)
      single_color_setup();
  }
}
