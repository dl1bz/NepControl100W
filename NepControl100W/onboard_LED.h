//----------------------------------------------------------------------------------------------
//    stuff for using the onboard LED of the ESP32-S3 Zero
//----------------------------------------------------------------------------------------------

// define things for onboard LED at GPIO PIN 21 (fix)

//-----------------------------------------------------------------------------------------
void setLED(const char *color) {
      const int LED_intensity = 10; // between 1..255
      if (color == "b") {
        neopixelWrite(ledPIN,0,0,LED_intensity); // LED blue, order GRB, not RGB !!!
      } else if (color == "r") {
        neopixelWrite(ledPIN,0,LED_intensity,0); // LED red, order GRB, not RGB !!!
      } else if (color == "g") {
        neopixelWrite(ledPIN,LED_intensity,0,0); // LED green, order GRB, not RGB !!!
      } else {
        neopixelWrite(ledPIN,0,0,0);              // LED off, order GRB, not RGB !!!
      }
  delay(50);                                      // delay after switch LED
}
//-----------------------------------------------------------------------------------------
