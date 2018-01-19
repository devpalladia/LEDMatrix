////////////////////////////////////////////////////////////////////////////////
// SERIAL PORT FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

void serialSetup() {
  // Set up serial port
  Serial.begin(BAUDRATE_SL);
}


void serialLoop() {
  // Process any incoming characters from the serial port
  while (Serial.available() > 0) {
    char c = Serial.read();
    // Add any characters that aren't the end of a command (semicolon) to the input buffer
    if (c != ';') {
      c = toupper(c);
      strncat(commandBuffer, &c, 1);
    }
    else
    {
      // Parse the command because an end of command token was encountered
      parseCommand(commandBuffer);

      // Clear the input buffer
      memset(commandBuffer, 0, sizeof(commandBuffer));
    }
  }
}
