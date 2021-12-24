/*
  ESP morse decoder

  Wire up the LDR and a 100 K resistor in series to the 5 V supply as a voltage divider, hooking up the center contact to the ADC input

    ┌─────────│+5V│
    │
  LDR 
    │
    ├─────────│ADC│
    │
  100K
    │
    └─────────│GND│
*/

#include <Arduino.h>

// Constants
#define THRESHOLD     400;  // Threshold between 0 and 1 on ADC
#define SHORT_LONG    500;  // Threshold between short and long pulse in ms
#define TIME_WORDS    800;  // Threshold to detect a new word in ms
//#define INVERT            // Define if you need to invert HIGH and LOW

// Variables
int  brightness     =   0;  // Measured brightness
bool signal;                // Measured signal HIGH or LOW


/***********************************    Setup    ******************************************/
void setup() {

  // Start serial monitor
  Serial.begin(115200);

}

/****************************************     Loop     **********************************************/
void loop() {

  // Read in brightness level
  brightness = analogRead(A0);

  // Detect HIGH or LOW signal
  #ifndef INVERT
    signal = brightness > THRESHOLD;
  #else
    signal = brightness < THRESHOLD;
  #endif

  Serial.print("Signal: ");
  Serial.print(signal);
  Serial.print(" | Brightness: ");
  Serial.println(brightness);

}
