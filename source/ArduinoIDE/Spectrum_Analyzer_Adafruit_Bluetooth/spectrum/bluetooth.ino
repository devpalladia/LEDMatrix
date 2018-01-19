////////////////////////////////////////////////////////////////////////////////
// BLUETOOTH FUNCTIONS
////////////////////////////////////////////////////////////////////////////////

//Serial1 Pin0(RX) Pin1(TX)
//Serial2 Pin9(RX) Pin10(TX)
//Serial3 Pin7(RX) Pin8(TX)
#define bluetooth Serial1

void bluetoothSetup() {
  // Set up for Bluetooth Mate Gold Modul
  bluetooth.begin(BAUDRATE_BT);  // The Bluetooth Mate defaults to 115200bps
}

void bluetoothLoop() {
  // Process any incoming characters from the bluetooth port
  while (bluetooth.available() > 0) {
    char c = bluetooth.read();
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

